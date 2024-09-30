// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieBaseAnimeIns.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UZombieBaseAnimeIns : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeValue")
	float movementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeValue")
	float hitWeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeValue")
	int runIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeMontage")
	class UAnimMontage* attackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeMontage")
	class UAnimMontage* wakeUpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimeMontage")
	bool bIsFall;

private:
	class AZombieBase* owner;
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override;
};
