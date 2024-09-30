// Fill out your copyright notice in the Desc,iption page of Project Settings.


#include "Charger.h"
#include <Engine/SkeletalMesh.h>
#include <Components/CapsuleComponent.h>
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include "Perception/PawnSensingComponent.h"
#include <AIModule/Classes/AIController.h>
#include "Net/UnrealNetwork.h"
#include <EngineUtils.h>
#include "LeftCharacter.h"
#include "Camera/CameraShakeBase.h"
#include "ChargerAnimInstance.h"
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ACharger::ACharger()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/KJS/Charger/Charger/charger2.charger2'"));

	if (TempMesh.Succeeded())
	{
		//스켈레탈메쉬 넣어주기
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(0, 0, -102), FRotator(0, -90, 0));
		GetMesh()->SetWorldScale3D(FVector(3.f));
	}
	GetCapsuleComponent()->SetCapsuleRadius(45.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(125.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ChargerRange = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ChargerRange"));
	ChargerRange->SetupAttachment(GetMesh(), TEXT("PlayerAttachSocket"));
	ChargerRange->SetRelativeLocation(FVector(10, 0, 0));
	ChargerRange->SetRelativeRotation(FRotator(100, 0, 0));
	ChargerRange->SetRelativeScale3D(FVector(0.65f, 0.65f, 0.75f));

	//BasicAttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	//BasicAttackSphere->SetupAttachment(GetMesh(), TEXT("AttackSocket"));

	Sight = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sight"));
	Sight->SetPeripheralVisionAngle(60.0f);
	Sight->SightRadius = 5000.f;

	if (GetWorld())
	{
		ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Charger(TEXT("//Script/Engine.AnimBlueprint'/Game/Dev/KJS/Charger/ChargerAnimation/ABP_Charger.ABP_Charger'"));
		if (ABP_Charger.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(ABP_Charger.Class);
		}
	}

	//ConstructorHelpers::FClassFinder<UCameraShakeBase> TempcameraShake(TEXT("/Script/Engine.Blueprint'/Game/Dev/KJS/Charger/BP/ChagerDampCameraShake.ChagerDampCameraShake_c'"));
	//if (TempcameraShake.Succeeded())
	//{
	//	ChargerShake = TempcameraShake.Class;
	//}

	ChargerSound = CreateDefaultSubobject<USoundBase>(TEXT("ChargerSound"));
	ChargerBreakSound = CreateDefaultSubobject<USoundBase>(TEXT("BreakSound"));
	ChargerDieSound = CreateDefaultSubobject<USoundBase>(TEXT("ChargerDie"));
	ChargerGrab = CreateDefaultSubobject<USoundBase>(TEXT("ChargerGrab"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ACharger::BeginPlay()
{
	Super::BeginPlay();

	currentHp = maxHp;
	//AI할당
	AI = Cast<AAIController>(Controller);

	//navsystem 추적
	navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	anim = Cast<UChargerAnimInstance>(GetMesh()->GetAnimInstance());

	ChargerRange->OnComponentBeginOverlap.AddDynamic(this, &ACharger::OnComponentBeginOverlap);
	//PawnSensing 넣어주기
	Sight->OnSeePawn.AddDynamic(this, &ACharger::OnseePlayer);

}

// Called every frame
void ACharger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//플레이어 인지범위
	float dis = 2000;
	//월드에 있는 플레이어 받아오기
	for (TActorIterator<ALeftCharacter> It(GetWorld(), ALeftCharacter::StaticClass()); It; ++It)
	{
		ALeftCharacter* Actor = *It;
		//플레이어와 거리 구하기
		float actorDis = (Actor->GetActorLocation() - GetActorLocation()).Size();
		//높이 받아오기
		float Zdis = FMath::Abs((Actor->GetActorLocation().Z - GetActorLocation().Z));
		if (dis > actorDis && Zdis < 100)
		{
			Target = Actor;
			dis = actorDis;
		}
	}

	switch (State)
	{
	case EChargerState::IDLE:
		Idle();
		break;
	case EChargerState::MOVE:
		Move();
		break;
	case EChargerState::ChargeATTACK:
		ChargeAttack();
		break;
	case EChargerState::ATTACK:
		BasicAttack();
		break;

	default:
		break;
	}


}

// Called to bind functionality to input
void ACharger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ChargerRange)
	{
		CatchPlayer = true;
		ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(OtherActor);
		if (LeftCharacter)
		{	
			if(!AttachedActorSet.Contains(LeftCharacter))
			{
				AttachedActorSet.Add(LeftCharacter);
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
				LeftCharacter->AttachToComponent(ChargerRange, AttachmentRules);
				LeftCharacter->GetCharacterMovement()->DisableMovement();
				UGameplayStatics::PlaySound2D(GetWorld(), ChargerGrab);

			}
		}
	}
}



void ACharger::Idle()
{
	if(HasAuthority())
	{
		if (Distance < 1800)
		{
			UGameplayStatics::PlaySound2D(GetWorld(),ChargerSound);
		}
		if (Target && !ActiveDash)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();
			if (Distance <= 1500)
			{
				State = EChargerState::ChargeATTACK;
				anim->ChargerState = State;
			}
		}
		MulticastIdle();
	}
}



void ACharger::ChargeAttack()
{
	if(HasAuthority())
	{
		if (AI)
		{
			if (Target && !ActiveDash)
			{
				//속도 올려
				GetCharacterMovement()->MaxWalkSpeed = 1000.f;
				//AI 움직이기
				//EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
				FVector Direction = Target->GetActorLocation() - GetActorLocation();
				FRotator TargetRotation = Direction.Rotation();
				SetActorRotation(TargetRotation);
				//방향만 구하기
				//FVector MoveDirection = Direction.GetSafeNormal();

				//Dash지점까지 떨어진 지점의 위치 계산
				FVector NewLocation = GetActorLocation() + Direction * DashDistance;
				//NewLocation.Z = GetActorLocation().Z;
				EPathFollowingRequestResult::Type result = AI->MoveToLocation(NewLocation);
			}


			if (CatchPlayer)
			{
				FVector ForwardVector = GetActorForwardVector();
				AddActorWorldOffset(ForwardVector * 2000 * GetWorld()->DeltaTimeSeconds, true);
				Smash();
				ActiveDash = true;
				//GetWorldTimerManager().SetTimer(ResetDashTimerHandle, this, &ACharger::ResetActivDash, DashCoolDownTime, false);
			}
		}
		MulticastChargeAttack();
	}

}


void ACharger::ResetActivDash()
{
	ActiveDash = false;
}

void ACharger::Move()
{
	if(HasAuthority())
	{
		if (AI)
		{
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
			EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
		}


		if (Target)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();

			if (Distance < AttackRange)
			{
				State = EChargerState::ATTACK;
				anim->ChargerState = State;
			}

		}
		MulticastMoveable();
	}
}


void ACharger::BasicAttack()
{
	if(HasAuthority())
	{
		if (Target)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();

			if (Distance < AttackRange)
			{
				anim->HandAttackMode = true;
			}

			if (Distance > AttackRange)
			{
				State = EChargerState::MOVE;
				anim->ChargerState = State;
			}
		}
		MulticastBasicAttack();
	}
}


void ACharger::Smash()
{
	if(HasAuthority())
	{
		FVector startPosition = GetActorLocation();
		FVector endPosition = startPosition + GetActorForwardVector() * 500;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(leftCharacter);
		//Params.AddIgnoredActor(Zombies);
		//Params.AddIgnored

		if (GetWorld()->LineTraceSingleByChannel(HitResult, startPosition, endPosition, ECC_Visibility, Params))
		{
			UGeometryCollectionComponent* HitGeoMetryCollectionComponent = Cast<UGeometryCollectionComponent>(HitResult.GetComponent());
			UStaticMeshComponent* HitStaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
			if (HitStaticMeshComponent || HitGeoMetryCollectionComponent)
			{

				DrawDebugLine(GetWorld(), startPosition, endPosition, FColor::Red, false, -1, 0, 1);
				if (HitResult.Component.IsValid())
				{
					if (CatchPlayer)
					{
						ActiveDash = false;
						for (AActor* AttachedActor : AttachedActorSet)
						{
							ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(AttachedActor);
							if (LeftCharacter)
							{
								CatchPlayer = false;
								FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
								AttachedActor->DetachFromActor(DetachmentRules);
								LeftCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
								ChargerRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
								UGameplayStatics::PlaySound2D(GetWorld(), ChargerBreakSound);
							}
						}
						if (AI)
						{
							AI->StopMovement();
							PlayAnimMontage(FallOff);
							GetWorldTimerManager().SetTimer(MovementResumeTimerHandle, this, &ACharger::ResumeAIMovement, 1.65f, false);
						}
						AttachedActorSet.Empty();
					}

				}
			}
		}
		MulticastSmash();
	}
}

//void ACharger::OnDmg(float Dmg)
//{
//	currentHp -= Dmg;
//
//	bIsSight = true;
//
//	UE_LOG(LogTemp, Warning, TEXT("Please Die"));
//
//	if (currentHp <= 0 && !isDie)
//	{
//		//MulticastDie();
//
//		isDie = true;
//
//		Die();
//	}
//}


void ACharger::Die()
{
	if (HasAuthority())
	{
		ServerDie_Implementation();
	}
	else
	{
		ServerDie();
	}
}

void ACharger::ResumeAIMovement()
{
	State = EChargerState::MOVE;
	anim->ChargerState = State;
}

void ACharger::IndicateSound()
{

}

void ACharger::OnseePlayer(APawn* Pawn)
{
	if (Target)
	{
		bIsSight = true;
		//MulticastSetIsSight(true);
	}
}


void ACharger::ServerOnDmg_Implementation(float Dmg)
{
	MulticastOnDmg(Dmg);
}

void ACharger::MulticastOnDmg_Implementation(float Dmg)
{
	currentHp -= Dmg;

	bIsSight = true;

	if (currentHp <= 0 && !isDie)
	{
		MulticastDie();

		isDie = true;
	}
}

void ACharger::ServerDie_Implementation()
{
	MulticastDie();
}

bool ACharger::ServerDie_Validate()
{
	return true;
}

void ACharger::MulticastDie_Implementation()
{
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetAnimClass(nullptr);

	UGameplayStatics::PlaySound2D(GetWorld(), ChargerDieSound);

	if (CatchPlayer)
	{
		ActiveDash = false;
		for (AActor* AttachedActor : AttachedActorSet)
		{
			ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(AttachedActor);
			if (LeftCharacter)
			{
				CatchPlayer = false;
				FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
				AttachedActor->DetachFromActor(DetachmentRules);
				LeftCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				ChargerRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			
		}
		AttachedActorSet.Empty();
	}
}

void ACharger::ServerIdle_Implementation()
{
	MulticastIdle();
}

bool ACharger::ServerIdle_Validate()
{
	return true;
}

void ACharger::MulticastIdle_Implementation()
{
	if (Target && !ActiveDash)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();
		if (Distance <= 1500)
		{
			State = EChargerState::ChargeATTACK;
			anim->ChargerState = State;
		}
	}
}

void ACharger::ServerChargeAttack_Implementation()
{
	MulticastChargeAttack();
}

bool ACharger::ServerChargeAttack_Validate()
{
	return true;
}

void ACharger::MulticastChargeAttack_Implementation()
{
	if (AI)
	{
		if (Target && !ActiveDash)
		{
			//속도 올려
			GetCharacterMovement()->MaxWalkSpeed = 1000.f;
			//AI 움직이기
			//EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			FRotator TargetRotation = Direction.Rotation();
			SetActorRotation(TargetRotation);
			//방향만 구하기
			//FVector MoveDirection = Direction.GetSafeNormal();

			//Dash지점까지 떨어진 지점의 위치 계산
			FVector NewLocation = GetActorLocation() + Direction * DashDistance;
			//NewLocation.Z = GetActorLocation().Z;
			EPathFollowingRequestResult::Type result = AI->MoveToLocation(NewLocation);
		}


		if (CatchPlayer)
		{
			FVector ForwardVector = GetActorForwardVector();
			AddActorWorldOffset(ForwardVector * 2000 * GetWorld()->DeltaTimeSeconds, true);
			Smash();
			ActiveDash = true;
			UGameplayStatics::PlaySound2D(GetWorld(), ChargerDieSound);
			//GetWorldTimerManager().SetTimer(ResetDashTimerHandle, this, &ACharger::ResetActivDash, DashCoolDownTime, false);
		}
	}
}

void ACharger::ServerMoveable_Implementation()
{
	MulticastMoveable();
}

bool ACharger::ServerMoveable_Validate()
{
	return true;
}

void ACharger::MulticastMoveable_Implementation()
{
	if (AI)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
	}


	if (Target)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();

		if (Distance < AttackRange)
		{
			State = EChargerState::ATTACK;
			anim->ChargerState = State;
		}

	}
}

void ACharger::ServerBasicAttack_Implementation()
{
	MulticastBasicAttack();
}

bool ACharger::ServerBasicAttack_Validate()
{
	return true;
}

void ACharger::MulticastBasicAttack_Implementation()
{
	if (Target)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();

		if (Distance < AttackRange)
		{
			anim->HandAttackMode = true;
		}

		if (Distance > AttackRange)
		{
			State = EChargerState::MOVE;
			anim->ChargerState = State;
		}
	}
}

void ACharger::ServerSmash_Implementation()
{
	MulticastSmash();
}

bool ACharger::ServerSmash_Validate()
{
	return true;
}

void ACharger::MulticastSmash_Implementation()
{
	FVector startPosition = GetActorLocation();
	FVector endPosition = startPosition + GetActorForwardVector() * 200;
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(leftCharacter);
	//Params.AddIgnoredActor(Zombies);
	//Params.AddIgnored

	if (GetWorld()->LineTraceSingleByChannel(HitResult, startPosition, endPosition, ECC_Visibility, Params))
	{
		UGeometryCollectionComponent* HitGeoMetryCollectionComponent = Cast<UGeometryCollectionComponent>(HitResult.GetComponent());
		UStaticMeshComponent* HitStaticMeshComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
		if (HitStaticMeshComponent || HitGeoMetryCollectionComponent)
		{

			DrawDebugLine(GetWorld(), startPosition, endPosition, FColor::Red, false, -1, 0, 1);
			if (HitResult.Component.IsValid())
			{
				if (CatchPlayer)
				{
					ActiveDash = false;
					for (AActor* AttachedActor : AttachedActorSet)
					{
						ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(AttachedActor);
						if (LeftCharacter)
						{
							CatchPlayer = false;
							FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
							AttachedActor->DetachFromActor(DetachmentRules);
							LeftCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
							ChargerRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}
					}
					if (AI)
					{
						AI->StopMovement();
						PlayAnimMontage(FallOff);
						GetWorldTimerManager().SetTimer(MovementResumeTimerHandle, this, &ACharger::ResumeAIMovement, 1.65f, false);
					}
					AttachedActorSet.Empty();
				}

			}
		}
	}
}

void ACharger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharger, currentHp);
	DOREPLIFETIME(ACharger, State);

}