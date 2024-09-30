// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Charger.h"
#include "ChargerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UChargerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	EChargerState ChargerState;

	UFUNCTION(BlueprintCallable)
	void OnHandAttackEvent();

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly)
	bool HandAttackMode = false;
	
};
