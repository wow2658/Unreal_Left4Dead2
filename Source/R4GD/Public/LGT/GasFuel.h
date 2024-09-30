// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "GasFuel.generated.h"

/**
 *
 */
UCLASS()
class R4GD_API AGasFuel : public AInteractionBase
{
	GENERATED_BODY()
	AGasFuel();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActiveCPP = true;

	//UFUNCTION()
	//void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerGrabWeapon(class ALeftCharacter* player);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGrabWeapon(class ALeftCharacter* player);

};
