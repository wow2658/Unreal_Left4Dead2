// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAttackNotifyState.h"
#include <Kismet/KismetSystemLibrary.h>
#include "ZombieBase.h"
#include "LeftCharacter.h"

void UZombieAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Owner = Cast<AZombieBase>(MeshComp->GetOwner());
}

void UZombieAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!Owner) return;

	FVector boneLocation;
	FHitResult hitResult;
	TArray<AActor*> ignoreActors;
	if (Owner->bIsRightAttack)
	{
		boneLocation = MeshComp->GetBoneLocation("hand_r");
	}
	else
	{
		boneLocation = MeshComp->GetBoneLocation("hand_l");
	}
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(Owner->GetWorld(), boneLocation, boneLocation, 20, TraceTypeQuery10, false, ignoreActors, EDrawDebugTrace::None, hitResult, true);
	if (bIsHit)
	{
		ALeftCharacter* hitActor = Cast<ALeftCharacter>(hitResult.GetActor());
		if (hitActor && !hitActors.IsValidIndex(hitActors.IndexOfByKey(hitActor)))
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), Owner->bIsRightAttack);
			hitActor->ServerOnDmg(1.f);
			hitActors.Add(hitActor);
		}
	}
}

void UZombieAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	hitActors.Empty();
}
