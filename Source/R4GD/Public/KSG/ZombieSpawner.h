// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class R4GD_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY()
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	TArray<TSubclassOf<AZombieBase>> zombieTypes;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	class UNavigationSystemV1* navSystem;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	bool bIsBeginSpawn;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	bool bIsUseNav = true;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	int count = 5;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	int wave = 5;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	float waveTimer;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	float waveTimerMax = 5;
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
	int currentWave = 0;
	UFUNCTION(BlueprintCallable)
	void OnSpawn();
};
