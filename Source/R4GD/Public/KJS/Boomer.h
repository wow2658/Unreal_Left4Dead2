// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boomer.generated.h"

UENUM(BlueprintType)
enum class EBoomerState : uint8
{
	IDLE UMETA(DisplayName = "Idle State"),
	MOVE,
	ATTACK
};

UCLASS()
class R4GD_API ABoomer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoomer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//Boomer State 만들기
	UPROPERTY(Replicated, EditDefaultsOnly, Category = BoomerInfo)
	EBoomerState State = EBoomerState::IDLE;

	//상태함수 만들기
	void Idle();
	void Move();
	void AttackState();

	//Animation 가져오기
	UPROPERTY()
	class UBoomerAnimInstance* anim;

	//필요속성 : 대기시간, 경과시간
	UPROPERTY()
	float currentTime = 0;
	UPROPERTY()
	float idleDelayTime = 5;

	//타겟 선정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACharacter* Target;

	//AttackState 필요속성
	//공격 거리
	float attackRange = 150;

public:
	UPROPERTY()
	class AAIController* AI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* BoomerAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereLeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BoomerVomit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* VomitRoot;
	

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnVomitBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnVomitEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool AttackMode = false;
	bool bMontagePlaying = false;
	bool bIsSight = false;
	bool BoomerSoundPlayed = false;
	bool BoomerRangeSound = true;

	bool HasApplyDamaged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* AnimAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoomerAnimation)
	class UAnimMontage* Dead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoomerAnimation)
	class UAnimMontage* VomitAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	class UNavigationSystemV1* navSystem;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* BoomerSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* BoomerSee;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* ChangedProcess;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* BoomerBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnSetting)
	class UPawnSensingComponent* Sight;

	UFUNCTION()
	void ResetVomitCoolTime();

	UFUNCTION(BlueprintCallable, Category=Sound)
	void IndicateSound();

	//BoomerInfo
	UPROPERTY(EditAnywhere, Category = BoomerInfo)
	float maxHp;
	
	UPROPERTY(EditAnywhere, Category = BoomerInfo)
	float Distance;
	UPROPERTY(EditAnywhere, Category = BoomerInfo)
	float VomitTime;
	UPROPERTY(EditAnywhere, Category = BoomerInfo)
	float VomitRange = 600;
	
	UPROPERTY(EditAnywhere, Category = BoomerInfo)
	float VomitAttackTime = 5.f;

	

public:
	//UFUNCTION(BlueprintCallable, Category=BoomerFunctions)
	//void OnDmg(float Dmg);

	UFUNCTION(BlueprintCallable, Category = BoomerFunctions)
	void Die();

	UFUNCTION(BlueprintCallable, Category = BoomerFunctions)
	void Vomit();

	UFUNCTION(BlueprintCallable, Category = BoomerFunctions)
	void BoomerAttack();

	UFUNCTION(BlueprintCallable, Category = BoomerFunctions)
	void OnseePlayer(APawn* Pawn);

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABoomerBombCollision> collision;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABoomerBombForce> forceClass;

	bool isDie = false;
	
	bool CanVomit = true;


public:
//서버 로직들
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = BoomerFunctions)
	void ServerOnDmg(float dmg);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastOnDmg(float dmg);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = BoomerFunctions)
	void ServerDie();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastDie();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = BoomerFunctions)
	void ServerIndicateSound();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastIndicateSound();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastSetIsSight(bool NewIsSight);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = BoomerFunctions)
	void ServerIdle();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastIdle();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = BoomerFunctions)
	void ServerMoveTarget();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastMoveTarget();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = BoomerFunctions)
	void ServerAttackState();

	UFUNCTION(NetMulticast,Reliable, BlueprintCallable, Category = BoomerFunctions)
	void MulticastAttackState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(EditAnywhere, Category= BoomerFunctions)
	FTimerHandle Reset;

	//현재 체력
	UPROPERTY(Replicated, EditAnywhere, Category = BoomerInfo)
	float currentHp = 60;
};