// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerBombCollision.h"
#include <Components/SphereComponent.h>
#include "LeftCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABoomerBombCollision::ABoomerBombCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	root=CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent=root;
	Sphere= CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(64.0f);
	Sphere->SetupAttachment(root);

	ConstructorHelpers::FObjectFinder<USoundBase>TempChange(TEXT("/Script/Engine.SoundWave'/Game/Dev/KJS/BoomerSound/ChangeProcessSound.ChangeProcessSound'"));
	if (TempChange.Succeeded())
	{
		//부머 Vomit공격 맞았을때 사운드
		ChangedProcess = TempChange.Object;
	}
}

// Called when the game starts or when spawned
void ABoomerBombCollision::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABoomerBombCollision::OnComponentBeginOverlap);
}

// Called every frame
void ABoomerBombCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoomerBombCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALeftCharacter* LeftCharacter = Cast<ALeftCharacter>(OtherActor);
	if (LeftCharacter != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Why it didnt Work"));
		LeftCharacter->ActivatePostProcessEffect();
		UGameplayStatics::PlaySound2D(GetWorld(), ChangedProcess);
	}

	this->Destroy();
}

