// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieBase.generated.h"

UENUM()
enum class EZOMBIESTATE : int8
{
	LOCOMOTION,
	HITREACTION,
	DEATH
};

UCLASS()
class R4GD_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ZombieSystem)
	class UPhysicalAnimationComponent* physicsAnimComp;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ZombieSystem)
	class UAudioComponent* audioComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ZombieSystem)
	TMap<FName, float> boneHpMap;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ZombieSystem)
	TArray<FName> bronkenBoneNames;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ZombieSystem)
	class USoundCue* hitSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ZombieSystem)
	class USoundCue* hitVoice;
	UPROPERTY(VisibleAnywhere, Category = ZombieSystem)
	class AAIController* aiController;
	UPROPERTY(VisibleAnywhere, Category = ZombieSystem)
	class ALeftCharacter* target;
	UPROPERTY(VisibleAnywhere, Category = ZombieSystem)
	class UZombieBaseAnimeIns* animIns;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ZombieSystem)
	float reChaseTimer;	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ZombieSystem)
	float targetDis;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = ZombieSystem)
	float hitReactionTimer;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ZombieSystem)
	bool bIsRightAttack;
	UPROPERTY(VisibleAnywhere, Category = ZombieSystem)
	bool bIsAIMove;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ZombieSystem)
	bool bIsLegBrocken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ZombieInfo)
	float maxHp;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = ZombieInfo)
	float currentHp;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = ZombieInfo)
	bool bisAutoFollow = true;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = ZombieInfo)
	bool bisFall = false;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = ZombieInfo)
	float reGroundTimer;

	UPROPERTY(VisibleAnywhere, Category = ZombieInfo)
	EZOMBIESTATE zombieState = EZOMBIESTATE::LOCOMOTION;
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = ZombieFunctions)
	void ServerOnDmg(float dmg, FVector dmgDir = FVector::ZeroVector, FName hitBoneName = "spine_01");

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = ZombieFunctions)
	void MulticastOnDmg(float dmg, FVector dmgDir = FVector::ZeroVector, FName hitBoneName = "spine_01");

	UFUNCTION(NetMulticast, Reliable)
	void MultiLegBroken();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
