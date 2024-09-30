// Fill out your copyright notice in the Description page of Project Settings.


#include "Boomer.h"
#include <Engine/SkeletalMesh.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "LeftCharacter.h"
#include <Kismet/KismetMathLibrary.h>
#include <AIModule/Classes/AIController.h>
#include <Components/SphereComponent.h>
#include "BoomerBombCollision.h"
#include "BoomerBombForce.h"
#include "Camera/CameraComponent.h"
#include <NavigationSystem/Public/NavigationSystem.h>
#include "Perception/PawnSensingComponent.h"
#include <Sound/SoundBase.h>
#include <Sound/SoundCue.h>
#include <EngineUtils.h>
#include "BoomerAnimInstance.h"
#include "Net/UnrealNetwork.h"	// 언리얼 네트워크 기능 사용을 위한 헤더


// Sets default values
ABoomer::ABoomer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/KJS/Boomer/Mesh/boomer2.boomer2'"));

	if (TempMesh.Succeeded())
	{
		//스켈레탈메쉬 넣어주기
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -125), FRotator(0, -90, 0));

		GetMesh()->SetRelativeScale3D(FVector(0.035f));
	}

	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(130.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//애니메이션 몽타주 가져오기
	ConstructorHelpers::FObjectFinder<UAnimMontage>Anim(TEXT("/Script/Engine.AnimMontage'/Game/Dev/KJS/Boomer/Animation/AM_Attack.AM_Attack'"));
	if (Anim.Succeeded())
	{
		AnimAttack = Anim.Object;
	}
	//애니메이션 블루프린트 할당하기
	ConstructorHelpers::FClassFinder<UBoomerAnimInstance>TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/KJS/Boomer/Animation/ABP_Boomer.ABP_Boomer_C'"));
	if (TempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}
	//LeftHandSphere 콜리전 설정
	SphereLeftHand = CreateDefaultSubobject<USphereComponent>(TEXT("SphereLeft"));
	SphereLeftHand->SetSphereRadius(32.f);
	SphereLeftHand->SetRelativeScale3D(FVector(0.5f));
	SphereLeftHand->SetupAttachment(GetMesh(), "LeftAttack");

	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	//Vomit 타격 캡슐 만들기
	BoomerVomit = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoomerVomitRange"));
	BoomerVomit->SetRelativeLocation(FVector(-25, -120, 0));
	BoomerVomit->SetRelativeRotation(FRotator (0, -10, 90));
	BoomerVomit->SetCapsuleHalfHeight(100.f);
	BoomerVomit->SetCapsuleRadius(30.f);
	BoomerVomit->SetupAttachment(GetMesh(),TEXT("HeadSocket"));
	BoomerVomit->SetCollisionProfileName(TEXT("Vomit"));

	//부머 근처에있을때 사운드
	ConstructorHelpers::FObjectFinder<USoundBase>TempSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/KJS/BoomerSound/The_Boomer_Sound_Idle.The_Boomer_Sound_Idle'"));
	if (TempSound.Succeeded())
	{
		BoomerSound = TempSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>TempChange(TEXT("/Script/Engine.SoundWave'/Game/Dev/KJS/BoomerSound/ChangeProcessSound.ChangeProcessSound'"));
	if (TempChange.Succeeded())
	{
		//부머 특수공격(Vomit 공격)
		ChangedProcess = TempChange.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>TempBomb(TEXT("/Script/Engine.SoundWave'/Game/Dev/KJS/BoomerSound/BoomerAfterDead.BoomerAfterDead'"));
	if (TempBomb.Succeeded())
	{
		//부머 터질때 사운드
		BoomerBoom = TempBomb.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase>TempSee(TEXT("/Script/Engine.SoundCue'/Game/Dev/KJS/BoomerSound/BoomerSee.BoomerSee'"));
	if (TempSee.Succeeded())
	{
		//부머 봤을때 사운드
		BoomerSee = TempSee.Object;
	}

	Sight = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sight"));
	Sight->SetPeripheralVisionAngle(60.0f);
	Sight->SightRadius = 5000.f;

	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABoomer::BeginPlay()
{
	Super::BeginPlay();
	//체력설정
	if (HasAuthority())
	{
		maxHp = currentHp;
	}
	//ai컨트롤러 바인딩
	AI = Cast<AAIController>(Controller);

	//navsystem 추적
	navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	//overlap이벤트 넣어주기
	SphereLeftHand->OnComponentBeginOverlap.AddDynamic(this, &ABoomer::OnComponentBeginOverlap);
	BoomerVomit->OnComponentBeginOverlap.AddDynamic(this, &ABoomer::OnVomitBeginOverlap);
	BoomerVomit->OnComponentEndOverlap.AddDynamic(this,&ABoomer::OnVomitEndOverlap);

	//PawnSensing 넣어주기
	Sight->OnSeePawn.AddDynamic(this, &ABoomer::OnseePlayer);
	//anim 가져오기
	anim = Cast<UBoomerAnimInstance>(GetMesh()->GetAnimInstance());

}

// Called every frame
void ABoomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//플레이어 인지범위
	float dis = 1500;
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
	case EBoomerState::IDLE:
		Idle();
		break;
	case EBoomerState::MOVE:
		Move();
		break;
	case EBoomerState::ATTACK:
		AttackState();
		break;
	
	default:
		break;
	}

	if (isDie)
	{
		return;
	}

	MulticastIndicateSound();

}

// Called to bind functionality to input
void ABoomer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoomer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(OtherActor);
		if (LeftCharacter != nullptr)
		{
			LeftCharacter->ServerOnDmg(1.f);
			HasApplyDamaged = true;
			
			//SphereLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//AttackMode = false;
		}
		
}

void ABoomer::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HasApplyDamaged = false;
}

void ABoomer::OnVomitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		
		ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(OtherActor);
		if (LeftCharacter != nullptr)
		{
			LeftCharacter->ActivatePostProcessEffect();
			UGameplayStatics::PlaySound2D(GetWorld(), ChangedProcess);
			CanVomit = false;
			UE_LOG(LogTemp, Warning, TEXT("Boomer!!!!!!"));
		}
}

void ABoomer::OnVomitEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(OtherActor);
	if (LeftCharacter != nullptr)
	{
		BoomerVomit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


//void ABoomer::OnDmg(float Dmg)
//{
//
//}

void ABoomer::Die()
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

void ABoomer::Vomit()
{
	// Vomit�� �� ������
	if (!CanVomit)
	{
		//BoomerVomit�� �ݸ����� NoCollision �ȴ�.
		BoomerVomit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABoomer::ResetVomitCoolTime()
{
	CanVomit = true;
}

void ABoomer::IndicateSound()
{
	if (HasAuthority())
	{
		ServerIndicateSound();
	}

	if (BoomerSoundPlayed) return;
	if (Target)
	{
		if (bIsSight)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), BoomerSound);
			UGameplayStatics::PlaySound2D(GetWorld(), BoomerSee);
			BoomerSoundPlayed = true;
		}
	}
}

void ABoomer::BoomerAttack()
{
	AttackMode = true;
}


void ABoomer::OnseePlayer(APawn* Pawn)
{
	if (Target)
	{
		bIsSight = true;
		MulticastSetIsSight(true);
	}
}




void ABoomer::Idle()
{
	//currentTime +=GetWorld()->DeltaTimeSeconds;
	//float Radius = 1500;
	//FNavLocation Randomlocation;
	if(HasAuthority())
	{
		if (Target)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();
			if (Distance <= 1500 && bIsSight)
			{
				State = EBoomerState::MOVE;
				anim->boomerState = State;
			}
		}
		MulticastIdle();
	}

	
	//currentTime = 0;
	//
	//if (navSystem->GetRandomPointInNavigableRadius(GetActorLocation(), Radius, Randomlocation))
	//{
	//	FTransform trans;
	//	trans.SetLocation(Randomlocation.Location);
	//	AI->MoveToLocation(Randomlocation.Location);
	//}
	//else
	//{
	//	State = EBoomerState::IDLE;
	//	anim->boomerState = State;
	//}
		
	////시야에 들어오면 Move 상태로 바꾸기
	//if (Sight && bIsSight)
}

void ABoomer::Move()
{
	if(HasAuthority())
	{
		if (AI)
		{
			//AI 움직이기
			EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
		}
		//else
		//	State = EBoomerState::IDLE;
		//anim->boomerState = State;

		if (Target && !isDie)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();

			if (Distance < 600 && CanVomit)
			{	
				if (AI)
				{
					AI->StopMovement();
				}
				
				if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(VomitAttack))
				{
					BoomerVomit->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					AI->StopMovement();
					PlayAnimMontage(VomitAttack);
					Vomit();
					CanVomit = false;
					GetWorldTimerManager().SetTimer(Reset, this, &ABoomer::ResetVomitCoolTime, 30.f, false);
					
				}
			}

			if (Distance < 150 && !CanVomit)
			{
				State = EBoomerState::ATTACK;
				anim->boomerState = State;

			}
		}

		MulticastMoveTarget();
	}
}

void ABoomer::AttackState()
{
	if(HasAuthority())
	{
		if (Target)
		{
			FVector Direction = Target->GetActorLocation() - GetActorLocation();
			Distance = Direction.Size();

			if (Distance <= attackRange)
			{
				anim->HandAttackMode = true;
			}

			if (Distance > attackRange)
			{
				State = EBoomerState::MOVE;
				anim->boomerState = State;
			}
		}
		MulticastAttackState();
	}

}


void ABoomer::ServerOnDmg_Implementation(float Dmg)
{
	MulticastOnDmg(Dmg);
}

void ABoomer::MulticastOnDmg_Implementation(float Dmg)
{
	currentHp -= Dmg;

	bIsSight = true;

	if (currentHp <= 0 && !isDie)
	{
		MulticastDie();

		isDie = true;

		UGameplayStatics::PlaySound2D(GetWorld(), BoomerBoom);
	}
}



void ABoomer::ServerDie_Implementation()
{
	MulticastDie();
}

bool ABoomer::ServerDie_Validate()
{
	return true;
}

void ABoomer::MulticastDie_Implementation()
{
	GetCharacterMovement()->DisableMovement();

	SphereLeftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoomerVomit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation());
	UWorld* World = GetWorld();
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABoomerBombCollision* Collision = World->SpawnActor<ABoomerBombCollision>(collision, GetActorTransform(), Param);

	ABoomerBombForce* Force = World->SpawnActor<ABoomerBombForce>(forceClass, GetActorTransform(), Param);

	GetMesh()->HideBoneByName(TEXT("ValveBiped_Bip01_Spine"), EPhysBodyOp::PBO_None);

	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetAnimClass(nullptr);
}



void ABoomer::ServerIndicateSound_Implementation()
{
	MulticastIndicateSound();
}

bool ABoomer::ServerIndicateSound_Validate()
{
	return true;
}

void ABoomer::MulticastIndicateSound_Implementation()
{
	if (BoomerSoundPlayed) return;
	if (Target)
	{
		if (bIsSight)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), BoomerSound);
			UGameplayStatics::PlaySound2D(GetWorld(), BoomerSee);
			BoomerSoundPlayed = true;
		}
	}
}

void ABoomer::MulticastSetIsSight_Implementation(bool NewIsSight)
{
	bIsSight = NewIsSight;
}



void ABoomer::ServerIdle_Implementation()
{
	MulticastIdle();
}

bool ABoomer::ServerIdle_Validate()
{
	return true;
}

void ABoomer::MulticastIdle_Implementation()
{
	if (Target)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();
		if (Distance <= 1500 && bIsSight)
		{
			State = EBoomerState::MOVE;
			anim->boomerState = State;
		}
	}
}

void ABoomer::ServerMoveTarget_Implementation()
{
	MulticastMoveTarget();
}

bool ABoomer::ServerMoveTarget_Validate()
{
	return true;
}

void ABoomer::MulticastMoveTarget_Implementation()
{
	if (AI)
	{
		//AI 움직이기
		EPathFollowingRequestResult::Type result = AI->MoveToLocation(Target->GetActorLocation());
	}
		//State = EBoomerState::IDLE;
		//anim->boomerState = State;

	if (Target && !isDie)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();

		if (Distance < 600 && CanVomit)
		{
			if (AI)
			{
				AI->StopMovement();
			}
			if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(VomitAttack))
			{
				BoomerVomit->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				PlayAnimMontage(VomitAttack);
				Vomit();
				CanVomit = false;
				GetWorldTimerManager().SetTimer(Reset, this, &ABoomer::ResetVomitCoolTime, 30.f, false);
				
			}
		}

		if (Distance < 200 && !CanVomit)
		{
			State = EBoomerState::ATTACK;
			anim->boomerState = State;

		}
	}
}

void ABoomer::ServerAttackState_Implementation()
{
	MulticastAttackState();
}

bool ABoomer::ServerAttackState_Validate()
{
	return true;
}

void ABoomer::MulticastAttackState_Implementation()
{
	if (Target)
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Distance = Direction.Size();

		if (Distance <= attackRange)
		{
			anim->HandAttackMode = true;
		}

		if (Distance > attackRange)
		{
			State = EBoomerState::MOVE;
			anim->boomerState = State;
		}
	}
}

void ABoomer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoomer, currentHp);
	DOREPLIFETIME(ABoomer, State);

}