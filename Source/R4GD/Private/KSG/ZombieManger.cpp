// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieManger.h"
#include "EngineUtils.h"
#include "LeftCharacter.h"

// Sets default values
AZombieManger::AZombieManger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieManger::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ALeftCharacter> It(GetWorld(), ALeftCharacter::StaticClass()); It; ++It)
	{
		ALeftCharacter* Actor = *It;
		if (Actor)
		{
			characterArray.Add(Actor);
		}
	}
	
}

// Called every frame
void AZombieManger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

