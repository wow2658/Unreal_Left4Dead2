// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BoomerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UBoomerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	EBoomerState boomerState;

	UFUNCTION(BlueprintCallable)
	void OnHandAttackEvent();

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	bool HandAttackMode = false;
	
};
