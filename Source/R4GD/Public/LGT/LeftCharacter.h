// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Logging/LogMacros.h"
#include "LeftCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;



UENUM()
enum class EPlayerHank : int8
{
	LOCOMOTION,
	HITREACTION,
	DEATH

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndGame_Camera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireVFX);

UCLASS(config = Game, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class R4GD_API ALeftCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS)
	class UStaticMeshComponent* TP_Gun_CP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	class USpringArmComponent* TP_Spring_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	class UCameraComponent* TP_Camera;
	/////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FPS)
	class UCameraComponent* FPSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FPS)
	class USkeletalMeshComponent* FPSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPS)
	class USkeletalMeshComponent* FP_Gun_CP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPS)
	class USphereComponent* Sphere_Muzzle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InterAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FlashLightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VoiceChatAction;

public:

	ALeftCharacter();
	APlayerController* pc = GetController<APlayerController>();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerFire();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire();

	UFUNCTION(Server, Unreliable)
	void ServerStopFire();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStopFire();


	void StopFire(const FInputActionValue& Value);
	void Inter(const FInputActionValue& Value);
	void FlashLight(const FInputActionValue& Value);

	UFUNCTION(Server, reliable)
	void ServerFlashLight();

	UFUNCTION(NetMulticast, reliable)
	void MulticastFlashLight();

	//���ѱ��
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	bool bisLight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class USpotLightComponent* SpotLight1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class USpotLightComponent* SpotLight2;


	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);
	void StartWalk(const FInputActionValue& Value);
	void StopWalk(const FInputActionValue& Value);
	void StartChat();
	void EndChat();

	UFUNCTION(BlueprintCallable)
	void MuteALL(class APlayerState* player);

	UFUNCTION(BlueprintCallable)
	void UNMuteALL(class APlayerState* player);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fire)
	FTimerHandle fireHandler;

	// Ingame ui ��Ϻ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	TSubclassOf<class UUserWidget> InGameWidgetFactory;

	// UKismetMathLibrary::MapRangeClamped(GetCharacterMovement()->Velocity.Length(), 0, 450, 5, 70);
	class UUserWidget* InGameWidgetUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	TSubclassOf<class UUserWidget> CrossHairFactory;

	// UKismetMathLibrary::MapRangeClamped(GetCharacterMovement()->Velocity.Length(), 0, 450, 5, 70);
	class UUserWidget* CrossHairUI;

	// �ѻ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mysettings)
	class USoundBase* fireSound;
	// ������ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mysettings)
	class USoundBase* lightonoffSound;

	// �ѱ�VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* FireEffectMuzzle1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* NiagaraComp1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* FireEffectMuzzle2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* NiagaraComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UPointLightComponent* MuzzleLight;

	////////////////////////////////

		//�Ϲ����� VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* BloodEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* ZombieBlood1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* BloodEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* ZombieBlood2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* BloodEffect3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* ZombieBlood3;

	//�θ� VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* BoomerBloodEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* BoomerBlood1;

	//�Ϲݺ� VFX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* StaticBulletEffect1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* StaticBullet1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraSystem* StaticBulletEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* StaticBullet2;

	/////////////////////////////////


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UPostProcessComponent* PostProcessSK;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class AZombieBase* Zombie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class ABoomer* Boomer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class ACharger* Charger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* fireMontage;

	//ü�¼����ϱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	float maxHp = 30.f;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	float currentHp = 10.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	int32 ammo = 0;

	UPROPERTY(Replicated)
	FString myName = "Test Name";

	UPROPERTY(Replicated)
	int32 myMeshNumber = 0;

	UPROPERTY(Replicated)
	FLinearColor myMeshColor = FLinearColor::White;

	TArray<FString> meshPathList = {TEXT("/Game/Dev/LGT/Michael_/Michael_Mesh/michael_fullbody_NoTX"),
									TEXT("/Game/Dev/LGT/Hank_Anim/SK_Hank_01"), 
									TEXT("/Game/Dev/LGT/Hank_Anim/SK_Mannequin")};

	enum ENetRole myLocalRole;
	enum ENetRole myRemoteRole;




	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Mysettings)
	void ServerOnDmg(float Dmg);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Mysettings)
	void MulticastOnDmg(float Dmg);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	bool isDie = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mysettings)
	class UNiagaraComponent* BloodSplash;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FEndGame_Camera dieHank_Call;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FFireVFX fireHank_Call;

	//KJS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PostProcess)
	class UMaterialInterface* mat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PostProcess)
	FPostProcessSettings PostProcessSettings;

	//����Ʈ ���μ��� ���� �����ϱ� bool
	   //boomerprocess Ȯ���ϱ�
	   //vomit ���� �������� true�� �ٲ��ش�
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Boomer)
	bool BoomerProcess = false;
	//Vomit���� Ȱ��ȭ�ð�
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Boomer)
	float VomitActivationTime = 0.f;
	//�θ� Vomit���� ���ӽð�
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Boomer)
	float VomitProcessDuration = 6.f;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Boomer)
	float currentTime;

	UFUNCTION(BlueprintCallable, Category = Vomit)
	void ActivatePostProcessEffect();
	UFUNCTION(BlueprintCallable, Category = Vomit)
	void DeactivePostProcessEffect();


	//CSK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gas)
	class AGasFuel* GasCPP;

	void InitializePlayer();

	///////////////Net
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
