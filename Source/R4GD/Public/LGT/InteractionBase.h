// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionBase.generated.h"

UCLASS()
class R4GD_API AInteractionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class ALeftCharacter* interPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInteractOutlineVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UserCustom)
	float interactionOutlineDistance = 0;
public:

	UFUNCTION(BlueprintCallable)
	virtual void OnInteraction(class ALeftCharacter* player);
	
	UFUNCTION(BlueprintCallable)
	virtual void CancleInteraction();
};