// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieBaseAnimeIns.h"
#include "ZombieBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UZombieBaseAnimeIns::NativeBeginPlay()
{
	owner = CastChecked<AZombieBase>(TryGetPawnOwner());
}

void UZombieBaseAnimeIns::NativeUpdateAnimation(float DeltaTime)
{
	if (IsValid(owner))
	{
		bIsFall = owner->GetMovementComponent()->IsFalling();
		FVector velocity = owner->GetVelocity();
		velocity.Z = 0;
		movementSpeed = velocity.Size()/owner->GetCharacterMovement()->MaxWalkSpeed;
		hitWeight = owner->hitReactionTimer/.2;
	}
}
