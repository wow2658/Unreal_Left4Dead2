// Fill out your copyright notice in the Description page of Project Settings.


#include "GasFuel.h"
#include "Components/BoxComponent.h"
#include "R4GD.h"
#include "Net/UnrealNetwork.h"
#include "Engine/StaticMeshActor.h"
#include "LeftCharacter.h"


AGasFuel::AGasFuel()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Dev/LGT/SM_MERGED_FuelTank.SM_MERGED_FuelTank'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat0(TEXT("/Script/Engine.Material'/Game/Art/JSE/ShoppingMall/M_RedOrange.M_RedOrange'"));
	if (tempMat0.Succeeded())
	{
		meshComp->SetMaterial(0, tempMat0.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial> tempMat1(TEXT("/Script/Engine.Material'/Game/Art/JSE/ShoppingMall/M_Feul_Cap.M_Feul_Cap'"));
	if (tempMat1.Succeeded())
	{
		meshComp->SetMaterial(0, tempMat1.Object);
	}

	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetSimulatePhysics(true);
	meshComp->bRenderCustomDepth = true;

	bReplicates = true;
	SetReplicateMovement(true);

}

void AGasFuel::BeginPlay()
{
	Super::BeginPlay();

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AGasFuel::OnComponentBeginOverlap);
}


void AGasFuel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGasFuel::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALeftCharacter* player = Cast<ALeftCharacter>(OtherActor);

	if (player != nullptr)
	{

		SetOwner(player);

		ServerGrabWeapon(player);


	}
}

void AGasFuel::ServerGrabWeapon_Implementation(class ALeftCharacter* player)
{
	MulticastGrabWeapon(player);
}

void AGasFuel::MulticastGrabWeapon_Implementation(class ALeftCharacter* player)
{
	TArray<ALeftCharacter*> Result;
	if (player != nullptr && player->GasCPP == nullptr)
	{
		AttachToActor(player, FAttachmentTransformRules::KeepRelativeTransform);
		meshComp->SetRelativeLocationAndRotation(FVector(50,0,30),FRotator(0,90,0));
		meshComp->SetSimulatePhysics(false);
	    bIsActiveCPP = false;
		player->GasCPP = this;
	}
}

