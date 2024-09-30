// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "LeftCharacter.h"
#include "Components/CapsuleComponent.h"
#include "ZombieBaseAnimeIns.h"
#include <AIModule/Classes/Navigation/PathFollowingComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "EngineUtils.h"
#include <Components/AudioComponent.h>
#include <Sound/SoundCue.h>
#include "Net/UnrealNetwork.h"
// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	physicsAnimComp = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimComponent"));
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	audioComp->SetupAttachment(GetMesh(), "head");
	maxHp = 10; //�÷��̾��� ���ݷ��� 5->10���� �ø�
	boneHpMap.Add("head", 0);
	boneHpMap.Add("spine_01", 10);
	boneHpMap.Add("spine_02", 10);
	boneHpMap.Add("spine_03", 10);
	boneHpMap.Add("upperarm_l", 10);
	boneHpMap.Add("upperarm_r", 10);
	boneHpMap.Add("lowerarm_l", 10);
	boneHpMap.Add("lowerarm_r", 10);
	boneHpMap.Add("hand_l", 10);
	boneHpMap.Add("hand_r", 10);
	boneHpMap.Add("thigh_l", 0);
	boneHpMap.Add("thigh_r", 0);
	boneHpMap.Add("calf_l", 0);
	boneHpMap.Add("calf_r", 0);
	boneHpMap.Add("foot_l", 0);
	boneHpMap.Add("foot_r", 0);

	SetReplicates(true);
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();
	currentHp = maxHp;
	animIns = Cast<UZombieBaseAnimeIns>(GetMesh()->GetAnimInstance());
	if (animIns)
	{
		int random = FMath::RandRange(0, 100);
		if (random > 90)
		{
			animIns->runIndex = 0;
		}
		else if (random > 70)
		{
			animIns->runIndex = 1;
		}
		else if (random > 40)
		{
			animIns->runIndex = 2;
		}
		else
		{
			animIns->runIndex = 3;
		}
		switch (animIns->runIndex)
		{
			case 0:
				GetCharacterMovement()->MaxWalkSpeed = 200;
			break;
			case 1:
				GetCharacterMovement()->MaxWalkSpeed = 400;
			break;
			case 2:
				GetCharacterMovement()->MaxWalkSpeed = 500;
			break;
			case 3:
				GetCharacterMovement()->MaxWalkSpeed = 700;
			break;
			case 4:
				GetCharacterMovement()->MaxWalkSpeed = 700;
			break;
		}
	}
	aiController = Cast<AAIController>(Controller);
	FPhysicalAnimationData physicData;
	physicData.BodyName = "spine_01";
	physicData.OrientationStrength = 700;
	physicData.AngularVelocityStrength = 350;
	physicData.PositionStrength = 500;
	physicData.VelocityStrength = 350;
	if (physicsAnimComp)
	{
		physicsAnimComp->SetSkeletalMeshComponent(GetMesh());
		physicsAnimComp->ApplyPhysicalAnimationSettingsBelow("spine_01", physicData);
		GetMesh()->SetAllBodiesBelowSimulatePhysics("spine_01", true);
	}

}

// Called every frame
void AZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (zombieState == EZOMBIESTATE::DEATH) return;

	if (!bIsLegBrocken)
	{
		if (GetMovementComponent()->IsFalling())
		{
			if(GetMovementComponent()->Velocity.Z > -100 && !bisFall)
			{
				GetMesh()->SetSimulatePhysics(true);
				bisFall = true;
				reGroundTimer  = 3;
			}
		}
		else
		{
			if(bisFall)
			{
				if (FMath::RandRange(0, 2) == 0)
				{
					if (HasAuthority())
					{
						MultiLegBroken();
					}
				}
				else
				{
					GetMesh()->SetSimulatePhysics(false);

					GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepWorldTransform);
					GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
					GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
					GetMovementComponent()->Activate();
					//if (animIns && animIns->wakeUpMontage)
					//{
					//	if(animIns)
					//	UE_LOG(LogTemp, Warning, TEXT("DoMontage"));
					//	animIns->Montage_Play(animIns->wakeUpMontage);
					//}
					bisFall = false;
				}
				//GetMovementComponent()->Deactivate();
			}
		}
	}
	//if (zombieState != EZOMBIESTATE::DEATH)
	//{
	//	if (bIsLegBrocken)
	//	{
	//		FName hitBoneName = "hand_r";
	//		target = Cast<ALeftCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ALeftCharacter::StaticClass()));
	//		if (target)
	//		{
	//			FVector impulse = target->GetActorLocation() - GetMesh()->GetComponentLocation() - FVector::UpVector*50 ;
	//			impulse.Normalize();
	//			if(FMath::RandBool())
	//				impulse = impulse*-1;
	//			if (FMath::RandBool())
	//				hitBoneName = "hand_r";
	//			else
	//				hitBoneName = "thigh_r";
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//				GetMesh()->AddImpulse(impulse * 80, hitBoneName);
	//			if (FMath::RandBool())
	//				hitBoneName = "hand_l";
	//			else
	//				hitBoneName = "thigh_l";
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//				GetMesh()->AddImpulse(impulse * 80, hitBoneName);
	//			if (FMath::RandBool())
	//				hitBoneName = "spine_01";
	//			else
	//				hitBoneName = "spine_03";
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//				GetMesh()->AddImpulse(impulse * 250, hitBoneName);
	//
	//			
	//
	//			if (FMath::RandBool())
	//				hitBoneName = "foot_r";
	//			else
	//				hitBoneName = "foot_l";
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//				GetMesh()->AddImpulse(impulse * 80, hitBoneName);
	//			/*FString string1;
	//			switch (FMath::RandRange(0,4))
	//			{
	//				case 0 : string1 = "index"; break;
	//				case 1 : string1 = "middle"; break;
	//				case 2 : string1 = "pinky"; break;
	//				case 3 : string1 = "ring"; break;
	//				case 4 : string1 = "thumb"; break;
	//			}
	//
	//
	//			FString string2 = FString::Printf(TEXT("_0%d"), FMath::RandRange(1, 3));
	//			FString string3 = FMath::RandBool() ? "_r" : "_l";
	//			FString string4 = string1 + string2 + string3;
	//			hitBoneName = FName(*string4);
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//				GetMesh()->AddImpulse(impulse * 80, hitBoneName);
	//			UE_LOG(LogTemp, Warning, TEXT("%d"), GetMesh()->IsSimulatingPhysics(hitBoneName));*/
	//
	//			impulse = target->GetActorLocation() - GetMesh()->GetComponentLocation();
	//			impulse.Normalize();
	//			impulse = impulse + FVector::UpVector*5;
	//			impulse.Normalize();
	//			hitBoneName = "head";
	//			if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	//			{
	//				GetMesh()->AddImpulse(impulse * 100, hitBoneName);
	//				
	//			}
	//		}
	//	}
	//	else
	//	{
	//		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("spine_01", .5 + hitReactionTimer*2);
	//	}
	//}
	if (zombieState != EZOMBIESTATE::LOCOMOTION)
	{
		if (zombieState == EZOMBIESTATE::HITREACTION)
		{
			
			hitReactionTimer -= DeltaTime;
			if (hitReactionTimer <= 0)
			{
				zombieState = EZOMBIESTATE::LOCOMOTION;
			}
		}
		return;
	}

	if(!aiController || bIsLegBrocken) return;

	//float targetDis;

	if (IsValid(target))
	{
		FVector fpsdir = target->FPSMesh->GetRelativeLocation();
		fpsdir.Z = 0;
		targetDis = (target->GetActorLocation() + fpsdir - GetActorLocation()).Size();
		if (bisAutoFollow && targetDis > 50)
		{
			FVector targetDir = (target->GetActorLocation() + fpsdir - GetActorLocation());
			targetDir.Z = 0;
			targetDir.Normalize();
			GetCharacterMovement()->AddInputVector(targetDir);
			aiController->SetControlRotation(targetDir.Rotation());
			SetActorRotation(targetDir.Rotation());
		}
		if (target && targetDis < 150 && !animIns->IsAnyMontagePlaying())
		{
			animIns->Montage_Play(animIns->attackMontage);
			animIns->Montage_JumpToSection(animIns->attackMontage->GetSectionName(bIsRightAttack + 1));
			bIsRightAttack = !bIsRightAttack;
			//UE_LOG(LogTemp, Warning, TEXT("%d"), bIsRightAttack);
		}
	}
	reChaseTimer -= DeltaTime;
	if (reChaseTimer < 0)
	{
		float dis = 10000;
		for (TActorIterator<ALeftCharacter> It(GetWorld(), ALeftCharacter::StaticClass()); It; ++It)
		{
			ALeftCharacter* Actor = *It;
			float actorDis = (Actor->GetActorLocation() - GetActorLocation()).Size();
			float Zdis = Actor->GetActorLocation().Z - GetActorLocation().Z;
			if (dis > actorDis && Zdis < 100)
			{
				target = Actor;
				dis = actorDis;
			}
		}
		if (target)
		{
			//UE_LOG(LogTemp, Warning, TEXT("asdasd"));
			if (aiController)
			{
				FVector fpsdir = target->FPSMesh->GetRelativeLocation();
				EPathFollowingRequestResult::Type result = aiController->MoveToLocation(target->GetActorLocation() + fpsdir);
				if (result == EPathFollowingRequestResult::RequestSuccessful)
				{
					bisAutoFollow = false;
				}
				else
				{
					bisAutoFollow = true;
				}
				//if (result == EPathFollowingRequestResult::RequestSuccessful)
				//{
				////	bIsAIMove = true;
				//	GetCharacterMovement()->AddInputVector(FVector::ZeroVector);
				////}
				////else
				//{                      
				//	bIsAIMove = false;
				//	aiController->StopMovement();
				//}
			}
			//else
			//{     
			//	bIsAIMove = false;
			//}
			reChaseTimer = FMath::Clamp(targetDis / 1000, 0.2, 1);
		}
		//if (!aiController || !bIsAIMove)
		//{
		//	
		//	FVector targetDir = (target->GetActorLocation() - GetActorLocation());
		//	targetDir.Z = 0;
		//	targetDir.Normalize();
		//	GetCharacterMovement()->AddInputVector(targetDir);
		//	//aiController->SetControlRotation(targetDir.Rotation());
		//	//SetActorRotation(targetDir.Rotation());
		//}
	}
}

// Called to bind functionality to input
void AZombieBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AZombieBase::ServerOnDmg_Implementation(float dmg, FVector dmgDir, FName hitBoneName)
{
	MulticastOnDmg(dmg, dmgDir, hitBoneName);
}
void AZombieBase::MulticastOnDmg_Implementation(float dmg, FVector dmgDir, FName hitBoneName)
{
	if (zombieState == EZOMBIESTATE::DEATH) return;

	if (dmgDir == FVector::ZeroVector)
	{
		dmgDir = -GetActorForwardVector();
	}
	//UE_LOG(LogTemp, Warning, TEXT("asdasd"));
	if (hitBoneName == "head")
	{
		dmg *= 4;
	}
	else if (hitBoneName == "spine_01")
	{
		dmg *= 1;
	}
	else if (hitBoneName == "spine_02" || hitBoneName == "spine_03")
	{
		dmg *= 1.25;
	}
	else
	{
		dmg *= .75;
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, GetMesh()->GetBoneLocation(hitBoneName), FMath::FRand());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitVoice, GetMesh()->GetBoneLocation(hitBoneName), FMath::FRand());
	if(boneHpMap.Contains(hitBoneName))
	{
		float* hp = boneHpMap.Find(hitBoneName);
		if (hp)
		{
			*hp -= dmg;
			if (*hp < 0)
			{
				GetMesh()->HideBoneByName(hitBoneName, EPhysBodyOp::PBO_None);
				if ((hitBoneName == "thigh_l" ||
					hitBoneName == "calf_l" ||
					hitBoneName == "foot_l" ||
					hitBoneName == "ball_l" ||
					hitBoneName == "thigh_r" ||
					hitBoneName == "calf_r" ||
					hitBoneName == "foot_r" ||
					hitBoneName == "ball_r") &&
					bIsLegBrocken == false)
				{
					if (HasAuthority())
					{
						MultiLegBroken();
					}
					//GetCharacterMovement()->bUseRVOAvoidance = false;
				}
			}
		}
	}
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("spine_01", 1);
	currentHp -= dmg;
	UE_LOG(LogTemp, Warning, TEXT("Hp : %d"), currentHp);
	if (currentHp <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPhysicalAnimationData physicData;
		physicData.BodyName = "spine_01";
		physicsAnimComp->ApplyPhysicalAnimationSettingsBelow("spine_01", physicData);
		GetMovementComponent()->Deactivate();
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("spine_01", 0);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetAnimClass(nullptr);
		zombieState = EZOMBIESTATE::DEATH;
		if(hitBoneName == "head")
		{
			GetMesh()->HideBoneByName(hitBoneName, EPhysBodyOp::PBO_None);
		}
		//GetCharacterMovement()->bUseRVOAvoidance = false;
	}
	else
	{
		zombieState = EZOMBIESTATE::HITREACTION;
		hitReactionTimer = .5;
	}
	if (aiController)
		aiController->StopMovement();
	if (GetMesh()->IsSimulatingPhysics(hitBoneName))
	GetMesh()->AddImpulse(dmgDir * 5500, hitBoneName);
	if (hitBoneName == "head")
	{
		GetMesh()->AddImpulse(FVector::UpVector * 5500, hitBoneName);
	}
}

void AZombieBase::MultiLegBroken_Implementation()
{
	if (zombieState == EZOMBIESTATE::DEATH) return;

	bIsLegBrocken = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FPhysicalAnimationData physicData;
	physicData.BodyName = "spine_01";
	physicsAnimComp->ApplyPhysicalAnimationSettingsBelow("spine_01", physicData);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("spine_01", 0);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAnimClass(nullptr);
	GetMovementComponent()->Deactivate();
}

void AZombieBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super:: GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieBase, currentHp);

}

