// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Elevator.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API AElevator : public AStaticMeshActor
{
	GENERATED_BODY()
	
	AElevator();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

	UPROPERTY(EditAnywhere)
	float Speed = 50;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector GlobalTargetLocation;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FVector GlobalStartLocation;

	UPROPERTY(EditAnywhere, Category = Trigger)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = Tirrger)
	bool Ready = false;

	UFUNCTION()
  	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//UPROPERTY(EditAnywhere, Category= Component)
	//class UStaticMeshComponent* MeshComp;

};
