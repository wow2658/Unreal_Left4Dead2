// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Components/PostProcessComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "R4GD.h"
#include "ZombieBase.h"
#include "Boomer.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Animation/AnimInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Scene.h"
#include "Components/SpotLightComponent.h"
#include "Net/UnrealNetwork.h"
#include "R4GDGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Charger.h"
#include "Components/PointLightComponent.h"

ALeftCharacter::ALeftCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	SetReplicateMovement(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	TP_Spring_Arm= CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	TP_Spring_Arm->SetupAttachment(GetMesh());
	TP_Spring_Arm->SetRelativeLocationAndRotation(FVector(0,0,200), FRotator(0,90,0));
	TP_Spring_Arm->TargetArmLength = 300.f;
	TP_Spring_Arm->bAutoActivate = false;

	TP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	TP_Camera->SetupAttachment(TP_Spring_Arm);
	TP_Camera->SetRelativeRotation(FRotator(-30,0,0));
	TP_Camera->bAutoActivate = false;
	

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bRenderCustomDepth = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->CastShadow = true;
	

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;



	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSK(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/LGT/Michael_/Michael_Mesh/michael_fullbody_NoTX.michael_fullbody_NoTX'"));
	if (tempSK.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempSK.Object);
	}
	



	

	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->CastShadow = false;


	TP_Gun_CP=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TP_Gun_CPP"));
	TP_Gun_CP->SetupAttachment(GetMesh(), TEXT("gun_socket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempTPGun(TEXT("/Script/Engine.StaticMesh'/Game/Dev/CSK/Weapon/M4a1.M4a1'"));
	if (tempTPGun.Succeeded())
	{
		TP_Gun_CP->SetStaticMesh(tempTPGun.Object);
	}
	TP_Gun_CP->bRenderInDepthPass = true;
	TP_Gun_CP->bRenderCustomDepth = true;
	TP_Gun_CP->bOwnerNoSee = true;
	TP_Gun_CP->bOnlyOwnerSee = false;

////////////////////////////////////////////////FPS
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);
	FPSCamera->SetRelativeLocation(FVector(-10, 1.75, 55));
	FPSCamera->bUsePawnControlRotation = true;



	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FPSMesh->SetupAttachment(FPSCamera);
	FPSMesh->SetRelativeLocationAndRotation(FVector(-17.443438, -11.155713, -145.344963),FRotator(-1.317554, -1.011777, 3.273046));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempFPSMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/LGT/FPSArmGun/Arms/Mesh/Mesh_Arms.Mesh_Arms'"));
	if (tempFPSMesh.Succeeded())
	{
		FPSMesh->SetSkeletalMesh(tempFPSMesh.Object);
	}
	FPSMesh->bOwnerNoSee = false;
	FPSMesh->bOnlyOwnerSee = true;
	FPSMesh->CastShadow = false;


	FP_Gun_CP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun_CPP"));
	FP_Gun_CP->SetupAttachment(FPSMesh, TEXT("hand_lSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempFPGun(TEXT("/Script/Engine.SkeletalMesh'/Game/Dev/LGT/FPSArmGun/Gun/Gun.Gun'"));
	if (tempFPGun.Succeeded())
	{
		FP_Gun_CP->SetSkeletalMesh(tempFPGun.Object);
	}
	FP_Gun_CP->SetRelativeLocationAndRotation(FVector(-2.752708, 5.310328, -0.835661), FRotator(-1.503793, -7.706474, 0.19382));
	FP_Gun_CP->bOwnerNoSee = false;
	FP_Gun_CP->bOnlyOwnerSee = true;
	FP_Gun_CP->CastShadow = false;
	FP_Gun_CP->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Sphere_Muzzle = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	Sphere_Muzzle->SetRelativeLocationAndRotation(FVector(0.015578, 57.992371, 6.495215),FRotator(7.556748, -12.369912, 81.727601));
	Sphere_Muzzle->SetRelativeScale3D(FVector(0.11765, 0.11765, 0.114726));
	Sphere_Muzzle->SetupAttachment(FP_Gun_CP);

	//MuzzleLight->CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleLightComp"));
	//MuzzleLight->SetupAttachment(Sphere_Muzzle);
	//MuzzleLight->SetRelativeLocation(FVector(66.661875, -19.57214, -52.588017));
	//MuzzleLight->SetIntensity(0);
	
	///////////////////////////////////////input
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Dev/LGT/Hank_Input/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
	{
		DefaultMappingContext = tempIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempJumpAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Jump_Player.IA_Jump_Player'"));
	if (tempJumpAction.Succeeded())
	{
		JumpAction = tempJumpAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempMoveAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Move_Player.IA_Move_Player'"));
	if (tempMoveAction.Succeeded())
	{
		MoveAction = tempMoveAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempLookAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Look_Player.IA_Look_Player'"));
	if (tempLookAction.Succeeded())
	{
		LookAction = tempLookAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempFireAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Fire_Player.IA_Fire_Player'"));
	if (tempFireAction.Succeeded())
	{
		FireAction = tempFireAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempInterAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_InterAction_Player.IA_InterAction_Player'"));
	if (tempInterAction.Succeeded())
	{
		InterAction = tempInterAction.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> tempFlashLightAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_FlashLight.IA_FlashLight'"));
	if (tempFlashLightAction.Succeeded())
	{
		FlashLightAction = tempFlashLightAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempCrouchAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Ctrl_Player.IA_Ctrl_Player'"));
	if (tempCrouchAction.Succeeded())
	{
		CrouchAction = tempCrouchAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempWalkAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_Shift_Player.IA_Shift_Player'"));
	if (tempWalkAction.Succeeded())
	{
		WalkAction = tempWalkAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tempChatAction(TEXT("/Script/EnhancedInput.InputAction'/Game/Dev/LGT/Hank_Input/IA_VoiceChat.IA_VoiceChat'"));
	if (tempChatAction.Succeeded())
	{
		VoiceChatAction = tempChatAction.Object;
	}

	//�ִϸ��̼� ���?����
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if (GetWorld())
	{
		ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Hank(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/LGT/Hank_BP/ABP_Hank.ABP_Hank_C'"));
		if (ABP_Hank.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(ABP_Hank.Class);
		}
	}
	

	static ConstructorHelpers::FClassFinder<UAnimInstance> FirstPerson_AnimBP(TEXT("/Script/Engine.AnimBlueprint'/Game/Dev/LGT/FirstPerson/Animations/FirstPerson_AnimBP.FirstPerson_AnimBP_C'"));
	if (FirstPerson_AnimBP.Succeeded())
	{
		FPSMesh->SetAnimInstanceClass(FirstPerson_AnimBP.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Dev/LGT/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		fireMontage = ATTACK_MONTAGE.Object;
	}	
	
	


///////////////////////////////////////////////////////

	PostProcessSK = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessSK"));

	// �ѻ���
	ConstructorHelpers::FObjectFinder<USoundBase> TempSound(TEXT("/Script/Engine.SoundCue'/Game/Dev/LGT/FPSAssests/Sounds/Sound_Packs/Survival_Sounds_Selected_Sounds/Cues/Assault_Rifle_Shot_3_Cue.Assault_Rifle_Shot_3_Cue'"));
	if (TempSound.Succeeded())
	{
		fireSound = TempSound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> TemplightonoffSound(TEXT("/Script/Engine.SoundWave'/Game/Dev/LGT/Audio/flashlight1.flashlight1'"));
	if (TemplightonoffSound.Succeeded())
	{
		lightonoffSound = TemplightonoffSound.Object;
	}
	
	//ConstructorHelpers::FObjectFinder<UParticleSystem> TempBulletFX(TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/LGT/FPSAssests/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_02.NS_AR_Muzzle_flashes_02'"));
	//if (TempBulletFX.Succeeded())
	//{
	//	muzzleEffectFactory = TempBulletFX.Object;
	//}
	//

	ConstructorHelpers::FClassFinder<UCameraShakeBase> TempcameraShake(TEXT("/Script/Engine.Blueprint'/Game/Dev/LGT/Hank_BP/BP_FireCameraShake.BP_FireCameraShake_C'"));
	if (TempcameraShake.Succeeded())
	{
		cameraShake = TempcameraShake.Class;
	}

	FireEffectMuzzle1 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/LGT/FPSAssests/BluePrints/Player/Material/NS_AR_Muzzle_flashes_01.NS_AR_Muzzle_flashes_01'"));

	FireEffectMuzzle2 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/LGT/FPSAssests/Gun_VFX/Niagara_FX/Muzzle_Flashes/NS_AR_Muzzle_flashes_02.NS_AR_Muzzle_flashes_02'"));	

	///ZombieBlood1
	BloodEffect1 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Parts/P_E_Explosion_Splash_03.P_E_Explosion_Splash_03'"));

	///ZombieBlood2
	BloodEffect2 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Parts/P_E_Blood_HitAndSmoke_02.P_E_Blood_HitAndSmoke_02'"));

	///ZombieBlood3
	BloodEffect3 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Systems/P_BigSplash_Hit.P_BigSplash_Hit'"));

	///BoomerBlood1
	BoomerBloodEffect1 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Parts/P_E_Blood_SmokeDroplets.P_E_Blood_SmokeDroplets'"));

	//StaticBullet1
	StaticBulletEffect1 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Parts/P_E_Sparks_01.P_E_Sparks_01'"));

	//StaticBullet2
	StaticBulletEffect2 = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Dev/Blood_VFX_Pack/Particles/Parts/P_E_Sparks_02.P_E_Sparks_02'"));



	////////////////////////////////////////////////////�����ȿ��?
	BloodSplash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("niagara"));
	BloodSplash->SetupAttachment(GetMesh(),TEXT("clavicle_l"));

	//�÷��̾� fps camera�� Set PostProcess Material �Ҵ��ϱ�
	ConstructorHelpers::FObjectFinder<UMaterialInterface>TempMat(TEXT("/Script/Engine.Material'/Game/Dev/KJS/PostProcess/M_BoomerPostProcess.M_BoomerPostProcess'"));
	if (TempMat.Succeeded())
	{
		mat = TempMat.Object;
	}


	PostProcessSettings = FPostProcessSettings();
	PostProcessSettings.WeightedBlendables.Array.Add(FWeightedBlendable(1.0f, mat));
	FPSCamera->PostProcessSettings = PostProcessSettings;


	SpotLight1 = CreateDefaultSubobject<USpotLightComponent> (TEXT("SPotLight_1"));
	SpotLight2 = CreateDefaultSubobject<USpotLightComponent> (TEXT("SPotLight_2"));
	SpotLight1->SetupAttachment(Sphere_Muzzle);
	
	SpotLight1->SetRelativeRotation(FRotator(75, 15.859169, 17.281712));
	SpotLight2->SetupAttachment(SpotLight1);

	SpotLight1->SetVisibility(false);
	SpotLight1->SetIntensity(5000.f);
	SpotLight1->SetOuterConeAngle(35.f);
	SpotLight2->SetVisibility(false);
	SpotLight2->SetIntensity(10000.f);
	SpotLight2->SetOuterConeAngle(15.f);
	


	////////�����ڳ� ������
}

void ALeftCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	currentHp = maxHp;

	FPSCamera->SetPostProcessBlendWeight(0.0f);



	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		InGameWidgetUI = CreateWidget<UUserWidget>(GetWorld(), InGameWidgetFactory);
	

		if (InGameWidgetUI != nullptr)
		{
			InGameWidgetUI->AddToViewport();		
		}
	}

	//TArray<AMyClass*> MyActors;
	//TArray<AActor*> OutActors;

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyClass::StaticClass(), OutActors);
	//for (AActor* a : OutActors)
	//{
	//	MyActors.Add(Cast<AMyClass>(a));
	//}

	if (HasAuthority())
	{
		currentHp = maxHp;
	}

	FTimerHandle initHandler;
	GetWorldTimerManager().SetTimer(initHandler, this, &ALeftCharacter::InitializePlayer, 1.0f, false);
}

void ALeftCharacter::InitializePlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Init Value: %s, %d"), *myName, myMeshNumber);

	//// �޽� ����
	//USkeletalMesh* selectedMesh = LoadObject<USkeletalMesh>(NULL, *meshPathList[myMeshNumber], NULL, LOAD_None, NULL);
	//if (selectedMesh != nullptr)
	//{
	//	GetMesh()->SetSkeletalMesh(selectedMesh);
	//}

}


void ALeftCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	dieHank_Call.Clear();
	fireHank_Call.Clear();
}

void ALeftCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BoomerProcess == true)
	{
		currentTime += DeltaTime;
		if (currentTime >= VomitProcessDuration)
		{
			DeactivePostProcessEffect();
			currentTime = 0;
		}
	}
	
	

}


void ALeftCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALeftCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALeftCharacter::Look);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ALeftCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ALeftCharacter::StopFire);
		EnhancedInputComponent->BindAction(InterAction, ETriggerEvent::Started, this, &ALeftCharacter::Inter);
		EnhancedInputComponent->BindAction(FlashLightAction, ETriggerEvent::Started, this, &ALeftCharacter::FlashLight);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ALeftCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ALeftCharacter::StopCrouch);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &ALeftCharacter::StartWalk);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &ALeftCharacter::StopWalk);
		EnhancedInputComponent->BindAction(VoiceChatAction, ETriggerEvent::Started, this, &ALeftCharacter::StartChat);
		EnhancedInputComponent->BindAction(VoiceChatAction, ETriggerEvent::Completed, this, &ALeftCharacter::EndChat);

	}

}

void ALeftCharacter::Move(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	
	}
}

void ALeftCharacter::Look(const FInputActionValue& Value)
{
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALeftCharacter::Fire(const FInputActionValue& Value)
{
	// RPC ���� ���?����
	if (HasAuthority())
	{
		ServerFire_Implementation();
	}
	else
	{
		ServerFire();
	}
	
}

void ALeftCharacter::ServerFire_Implementation()
{
	MulticastFire();
}

bool ALeftCharacter::ServerFire_Validate()
{
	return true;
}

void ALeftCharacter::MulticastFire_Implementation()
{
	GetWorldTimerManager().SetTimer(fireHandler, FTimerDelegate::CreateLambda([&]() {
		float VolumeMulti = UKismetMathLibrary::RandomFloatInRange(0.8,1.2);
		float PitchMulti = VolumeMulti-0.5;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation(), VolumeMulti);
		if (fireHank_Call.IsBound() == true)
		{
			fireHank_Call.Broadcast();
		}
	/*	auto pc = GetWorld()->GetFirstPlayerController();
		pc->PlayerCameraManager->StartCameraShake(cameraShake);*/

		APlayerController* pc = GetController<APlayerController>();

		if (pc != nullptr && pc->IsLocalPlayerController())
		{
			
			pc->ClientStartCameraShake(cameraShake);
		}


		FPSMesh->GetAnimInstance()->Montage_Play(fireMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0, true);

		UNiagaraComponent* NiagaraComp1 = UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle1, FP_Gun_CP, TEXT("Muzzle"), FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		UNiagaraComponent* NiagaraComp2 = UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle2, FP_Gun_CP, TEXT("Muzzle"), FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);




		FVector startPos1 = FPSCamera->GetComponentLocation();
		FVector endPos1 = startPos1 + FPSCamera->GetForwardVector() * 5000;
		FHitResult hitInfo1;
		FHitResult hitInfo2;
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);
		bool bHit1 = GetWorld()->LineTraceSingleByChannel(hitInfo1, startPos1, endPos1, ECC_Visibility, param);
		FVector startPos2 = Sphere_Muzzle->GetComponentLocation();
		FVector endPos2 = hitInfo1.ImpactPoint;
		bool bHit2 = GetWorld()->LineTraceSingleByChannel(hitInfo2, startPos2, endPos2, ECC_Visibility, param);

		if (bHit1 && bHit2)
		{

			FTransform trans;
			trans.SetLocation(hitInfo2.ImpactPoint);
			trans.SetRotation(hitInfo2.ImpactNormal.ToOrientationQuat());
			FRotator NiagaraME = UKismetMathLibrary::FindLookAtRotation(hitInfo2.ImpactPoint, GetActorLocation());

			if (hitInfo2.GetActor())
			{
				if (hitInfo2.GetActor()->IsA<AZombieBase>())
				{

					ZombieBlood1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect1, hitInfo2.ImpactPoint, NiagaraME);

					ZombieBlood2 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect2, hitInfo2.ImpactPoint, NiagaraME);

					ZombieBlood3 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect3, hitInfo2.ImpactPoint, NiagaraME);


					FVector Dmgdir = (hitInfo2.TraceEnd - hitInfo2.TraceStart);
					Dmgdir.Normalize();
					Zombie = Cast<AZombieBase>(hitInfo2.GetActor());
					Zombie->ServerOnDmg(10.0f, Dmgdir, hitInfo2.BoneName);
				}
				else if (hitInfo2.GetActor()->IsA<ABoomer>())
				{
					BoomerBlood1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoomerBloodEffect1, hitInfo2.ImpactPoint, NiagaraME);


					Boomer = Cast<ABoomer>(hitInfo2.GetActor());
					Boomer->ServerOnDmg(8.f);
				}
				else if (hitInfo2.GetActor()->IsA<ACharger>())
				{
					BoomerBlood1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoomerBloodEffect1, hitInfo2.ImpactPoint, NiagaraME);

					Charger = Cast<ACharger>(hitInfo2.GetActor());
					Charger->ServerOnDmg(8.f);
				}

				else
				{

					int32 randFX = FMath::RandRange(0, 2);
					switch (randFX)
					{
					case 0:
						StaticBullet1 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), StaticBulletEffect1, hitInfo2.ImpactPoint, NiagaraME);
						break;

					case 1:
						StaticBullet2 = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), StaticBulletEffect2, hitInfo2.ImpactPoint, NiagaraME);
						break;

					case 2:
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), StaticBulletEffect1, hitInfo2.ImpactPoint, NiagaraME);
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), StaticBulletEffect2, hitInfo2.ImpactPoint, NiagaraME);
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffectMuzzle2, hitInfo2.ImpactPoint, NiagaraME);
						break;
					}



					}

				}
			}


		}

		}), 0.1f, true); //���� �ӵ�
}

void ALeftCharacter::StopFire(const FInputActionValue& Value)
{

	ServerStopFire();
}

void ALeftCharacter::ServerStopFire_Implementation()
{
	MulticastStopFire();
}

void ALeftCharacter::MulticastStopFire_Implementation()
{
	GetWorldTimerManager().ClearTimer(fireHandler);
}



void ALeftCharacter::Inter(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{

	}
	
	
	
}

void ALeftCharacter::FlashLight(const FInputActionValue& Value)
{	
	if (Value.Get<bool>())
	{
		ServerFlashLight();
	}

}

void ALeftCharacter::ServerFlashLight_Implementation()
{
	MulticastFlashLight();
}

void ALeftCharacter::MulticastFlashLight_Implementation()
{
	if (!bisLight)
	{
		UE_LOG(LogTemp, Warning, TEXT("on"));
		SpotLight1->SetVisibility(true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lightonoffSound, GetActorLocation());
		bisLight = true;
		//SpotLight2->SetVisibility(true);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("off"));
		SpotLight1->SetVisibility(false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), lightonoffSound, GetActorLocation());
		bisLight = false;
		//SpotLight2->SetVisibility(false);
	}
}

void ALeftCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
	FPSCamera->SetRelativeLocation(FVector(0,0, 30));
}

void ALeftCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
	FPSCamera->SetRelativeLocation(FVector(0, 0, 74.106461));
}

void ALeftCharacter::StartWalk(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ALeftCharacter::StopWalk(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ALeftCharacter::StartChat()
{
	APlayerController* pcc = GetController<APlayerController>();
	if(pcc)
		pcc->StartTalking();
	else
	UE_LOG(LogTemp, Error, TEXT("DIEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));

}

void ALeftCharacter::EndChat()
{
	APlayerController* pcc = GetController<APlayerController>();
	if (pcc)
	pcc->StopTalking();
	else
	UE_LOG(LogTemp, Error, TEXT("DIEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));

}

void ALeftCharacter::MuteALL(class APlayerState* player)
{
	//FUniqueNetIdRepl playerId = player->GetUniqueId();
	//Cast<APlayerController>(GetController())->ServerMutePlayer(playerId);
	//pc->ServerMutePlayer(playerId);
	//if (bIsAllmute)
	TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
	for (APlayerState* playe : players)
	{
		pc->ServerMutePlayer(playe->GetUniqueId());
	}
}

void ALeftCharacter::UNMuteALL(class APlayerState* player)
{
	//FUniqueNetIdRepl playerId = player->GetUniqueId();
	TArray<APlayerState*> players = GetWorld()->GetGameState()->PlayerArray;
	for (APlayerState* playe : players)
	{
		pc->ServerUnmutePlayer(playe->GetUniqueId());
	}
}

void ALeftCharacter::ServerOnDmg_Implementation(float Dmg)
{
	MulticastOnDmg(Dmg);
}

void ALeftCharacter::MulticastOnDmg_Implementation(float Dmg)
{
	currentHp -= Dmg;

	if (currentHp <= 0 && !isDie)
	{


		
		if (dieHank_Call.IsBound() == true)
		{
			dieHank_Call.Broadcast();
			GetCharacterMovement()->DisableMovement();


			GetMesh()->SetOwnerNoSee(false);
			GetMesh()->SetOnlyOwnerSee(true);
			TP_Gun_CP->SetOwnerNoSee(false);
			TP_Gun_CP->SetOnlyOwnerSee(true);
		

			FPSMesh->SetVisibility(false);
			FP_Gun_CP->SetVisibility(false);

			FPSCamera->SetActive(false);
			TP_Camera->SetActive(true);
		}
		isDie = true;
		UE_LOG(LogTemp, Error, TEXT("DIEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE"));
	}
}

void ALeftCharacter::ActivatePostProcessEffect()
{
	BoomerProcess = true;
	FPSCamera->SetPostProcessBlendWeight(1.0f);
}

void ALeftCharacter::DeactivePostProcessEffect()
{
	BoomerProcess = false;

	FPSCamera->SetPostProcessBlendWeight(0.f);
}



void ALeftCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALeftCharacter, currentHp);
	DOREPLIFETIME(ALeftCharacter, isDie);
	DOREPLIFETIME(ALeftCharacter, bisLight);
	DOREPLIFETIME(ALeftCharacter, BoomerProcess);
	DOREPLIFETIME(ALeftCharacter, VomitActivationTime);
	DOREPLIFETIME(ALeftCharacter, VomitProcessDuration);
	DOREPLIFETIME(ALeftCharacter, currentTime);
	DOREPLIFETIME(ALeftCharacter, ammo);
	DOREPLIFETIME(ALeftCharacter, myName);
	DOREPLIFETIME(ALeftCharacter, myMeshNumber);
	DOREPLIFETIME(ALeftCharacter, myMeshColor);
}

