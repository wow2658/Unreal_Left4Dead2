// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoomerVomit.generated.h"

UCLASS()
class R4GD_API ABoomerVomit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoomerVomit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//총알 필요요소

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* VomitBulellet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BulletMesh;
};
