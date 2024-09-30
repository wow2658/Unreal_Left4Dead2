// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Charger.generated.h"

UENUM(BlueprintType)
 enum class EChargerState : uint8
{
	IDLE UMETA(DisplayName = "Idle State"),
	MOVE,
	ChargeATTACK,
	ATTACK
};

UCLASS()
class R4GD_API ACharger : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Boomer State 만들기
	UPROPERTY(Replicated, EditDefaultsOnly, Category = ChargerInfo)
	EChargerState State = EChargerState::IDLE;

	//타겟 선정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACharacter* Target;

	//타겟 선정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ALeftCharacter* leftCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AZombieBase* Zombies;

	UPROPERTY()
	class AAIController* AI;

	UPROPERTY(EditAnywhere, Category = Sight)
	class UNavigationSystemV1* navSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sight)
	class UPawnSensingComponent* Sight;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ChargerSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ChargerBreakSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ChargerDieSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ChargerGrab;

	//배열
	TSet<AActor*> AttachedActorSet;
	//카메라쉐이크
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChagerInfo)
	TSubclassOf<class UCameraShakeBase> ChargerShake;
public:
	////BoomerInfo
	//UPROPERTY(EditAnywhere, Category = ChargerInfo)
	//float maxHp;

	//상태함수 만들기
	void Idle();
	void Move();
	void ChargeAttack();
	void BasicAttack();
	void ResumeAIMovement();

	UFUNCTION(BlueprintCallable, Category=Sound)
	void IndicateSound();

	bool ActiveDash = false;
	bool CatchPlayer = false;
	bool isDie = false;
	bool bIsSight = false;
	bool ChargerSoundAttack = false;

	FVector DashDestination;

	UPROPERTY(EditAnywhere, Category = ChargerInfo)
	float Distance;
	UPROPERTY(EditAnywhere, Category = ChargerInfo)
	float DashDistance = 1500;
	UPROPERTY(EditAnywhere, Category = ChargerInfo)
	float AttackRange = 150;
	UPROPERTY(EditAnywhere, Category = ChargerInfo)
	float DashCoolDownTime = 30.f;

	//Animation 가져오기
	UPROPERTY()
	class UChargerAnimInstance* anim;


public:
	//Charger필요요소
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* ChargerRange;

	//Charger필요요소
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* BasicAttackSphere;
	//
	UFUNCTION(BlueprintCallable, Category = ChargerFunctions)
	void OnseePlayer(APawn* Pawn);
	//
	UFUNCTION(BlueprintCallable, Category = ChargerFunctions)
	void Die();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ResetActivDash();

	UFUNCTION()
	void Smash();

	//UFUNCTION(BlueprintCallable, Category=ChargerFunction)
	//void OnDmg(float Dmg);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharAnimation)
	class UAnimMontage* FallOff;
	//네트워크 로직

	//현재 체력
	UPROPERTY(Replicated,EditAnywhere, Category = ChargerInfo)
	float currentHp;

	UPROPERTY(EditAnywhere, Category = ChargerInfo)
	float maxHp = 130;
	//Dmg
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = ChargerFunctions)
	void ServerOnDmg(float Dmg);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastOnDmg(float Dmg);
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerDie();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastDie();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerIdle();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastIdle();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerChargeAttack();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastChargeAttack();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerMoveable();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastMoveable();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerBasicAttack();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastBasicAttack();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = ChargerFunctions)
	void ServerSmash();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = ChargerFunctions)
	void MulticastSmash();

	


	UPROPERTY(EditAnywhere, Category= ChargerFunction)
	FTimerHandle MovementResumeTimerHandle;

	UPROPERTY(EditAnywhere, Category= ChargerFunction)
	FTimerHandle ResetDashTimerHandle;
};
