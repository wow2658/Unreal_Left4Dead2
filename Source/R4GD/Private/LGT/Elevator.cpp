// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "R4GD.h"
#include "Engine/StaticMeshActor.h"


AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeScale3D(FVector(3,3,2));
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//MeshComp->SetupAttachmenat(BoxComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Dev/LGT/Eelevator_Floor.Eelevator_Floor'"));
	if (TempMesh.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(TempMesh.Object);
	}

	SetMobility(EComponentMobility::Movable);

	ConstructorHelpers::FObjectFinder<UMaterial> EVfloorMat(TEXT("/Script/Engine.Material'/Game/Art/JSE/ShoppingMall/M_WhieMarvel_3.M_WhieMarvel_3'"));
	if (EVfloorMat.Succeeded())
	{
		GetStaticMeshComponent()->SetMaterial(0,EVfloorMat.Object);
	}

	SetMobility(EComponentMobility::Movable);
	SetActorLocation(FVector(-1380,1470,540));
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlapBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnOverlapEnd);

	if (HasAuthority())
	{
	SetReplicates(true);
	SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

}

void AElevator::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	if (HasAuthority())
	{

		FVector Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		if (JourneyTravelled<= JourneyLength && Ready)
		{
			Location += Speed * Deltatime * Direction;
			SetActorLocation(Location);
		}

	}
	
}

void AElevator::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Ready = true;
}

void AElevator::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Ready = false;
}

