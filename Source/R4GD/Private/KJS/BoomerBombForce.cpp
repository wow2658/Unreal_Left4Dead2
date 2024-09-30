// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerBombForce.h"
#include <PhysicsEngine/RadialForceComponent.h>

// Sets default values
ABoomerBombForce::ABoomerBombForce()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root=CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent=root;

	Force=CreateDefaultSubobject<URadialForceComponent>(TEXT("Force"));
	Force->SetupAttachment(root);
	Force->Radius=400.0f;
	Force->bImpulseVelChange= true;
	Force->ImpulseStrength=1000.0f;
	Force->ForceStrength=500.0f;
}

// Called when the game starts or when spawned
void ABoomerBombForce::BeginPlay()
{
	Super::BeginPlay();

	Force->FireImpulse();
	
}

// Called every frame
void ABoomerBombForce::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

