// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBase.h"
#include "LeftCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
// Sets default values
AInteractionBase::AInteractionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionProfileName("InterObject");
	meshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	meshComp->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);


}

// Called when the game starts or when spawned
void AInteractionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsOutline = false;
	if (ALeftCharacter* player = Cast<ALeftCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		float playerDis = (player->GetActorLocation() - GetActorLocation()).Size();
		if (interactionOutlineDistance == 0 || interactionOutlineDistance > playerDis)
		{
			bIsOutline = true;
		}
	}
	if (!bIsInteractOutlineVisible) bIsOutline = false;
	meshComp->SetRenderCustomDepth(bIsOutline);
}

void AInteractionBase::OnInteraction(class ALeftCharacter* player)
{
	interPlayer = player;

}

void AInteractionBase::CancleInteraction()
{
	interPlayer = nullptr;
}

