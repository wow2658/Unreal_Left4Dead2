// Fill out your copyright notice in the Description page of Project Settings.


#include "R4GDLobbyModeBase.h"
#include "ServerLobbyController.h"
#include "R4GDGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "ServerLobbyPawn.h"

void AR4GDLobbyModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	AServerLobbyController* connectPlayer = Cast<AServerLobbyController>(NewPlayer);
	//AServerLobbyPawn* pawn = Cast<AServerLobbyPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//if(pawn)
	//{pawn->ServerUpdatePlayers();}
	//for (AServerLobbyController* con : connetedPlayers)
	//{
	//	con->ServerUpdatePlayers_Implementation();
	//	UE_LOG(LogTemp, Warning, TEXT("PC Name: %s"), *con->GetName());
	//}
	
	//if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	//OnLoginSuccessed.Broadcast();
}

void AR4GDLobbyModeBase::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);
	AServerLobbyController* exitingPlayer = Cast<AServerLobbyController>(ExitingPlayer);
	//connetedPlayers.RemoveAt(connetedPlayers.IndexOfByKey(exitingPlayer));
}

void AR4GDLobbyModeBase::BeginPlay()
{
	Super::BeginPlay();

	/*FTimerHandle updateHandler;
	GetWorld()->GetTimerManager().SetTimer(updateHandler, FTimerDelegate::CreateLambda([&]() {
		for (AServerLobbyController* con : connetedPlayers)
		{
			con->ServerUpdatePlayers_Implementation();
		}
		}), 1, true, 1);*/

	
}