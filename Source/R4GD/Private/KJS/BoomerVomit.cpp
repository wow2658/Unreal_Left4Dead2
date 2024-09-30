// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerVomit.h"
#include <Components/SphereComponent.h>

// Sets default values
ABoomerVomit::ABoomerVomit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VomitBulellet = CreateDefaultSubobject<USphereComponent>(TEXT("VomitBullet"));
	RootComponent = VomitBulellet;

}

// Called when the game starts or when spawned
void ABoomerVomit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoomerVomit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

