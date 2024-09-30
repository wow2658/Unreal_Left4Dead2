// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerLobbyController.h"

void AServerLobbyController::BeginPlay()
{
	Super::BeginPlay();

	pawn = Cast<AServerLobbyPawn>(GetPawn());
}