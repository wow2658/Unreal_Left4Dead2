// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawner.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "ZombieBase.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComp->SetupAttachment(RootComponent);

	//navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bIsBeginSpawn)
	OnSpawn();
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentWave > 0)
	{
		waveTimer -= DeltaTime;
		if (waveTimer < 0)
		{
			if (bIsUseNav)
			{
				navSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
				if (navSystem)
				{
					const FVector Origin = GetActorLocation();
					float Radius = 500.;
					FNavLocation RandomLocation;
					FActorSpawnParameters param;
					param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					for (int i = 0; i < count; i++)
					{
						//UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), Origin.X, Origin.Y, Origin.Z);

						if (navSystem->GetRandomPointInNavigableRadius(Origin, Radius, RandomLocation))
						{
							FTransform trans;
							trans.SetLocation(RandomLocation.Location);
							UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), RandomLocation.Location.X, RandomLocation.Location.Y, RandomLocation.Location.Z);
							GetWorld()->SpawnActor<AZombieBase>(zombieTypes[FMath::RandRange(0, zombieTypes.Num() - 1)], trans, param);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Failed to find a valid spawn location."));
						}
					}
				}
			}
			else
			{
				const FVector Origin = GetActorLocation();
				FActorSpawnParameters param;
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				for (int i = 0; i < count; i++)
				{
					FTransform trans;
					trans.SetLocation(Origin);
					GetWorld()->SpawnActor<AZombieBase>(zombieTypes[FMath::RandRange(0, zombieTypes.Num() - 1)], trans, param);
				}
			}
			waveTimer = waveTimerMax;
			currentWave--;
		}
	}
}

void AZombieSpawner::OnSpawn()
{
	currentWave = wave;
}
