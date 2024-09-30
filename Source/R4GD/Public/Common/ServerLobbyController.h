// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ServerLobbyController.generated.h"
/**
 * 
 */
UCLASS()
class R4GD_API AServerLobbyController : public APlayerController
{
	GENERATED_BODY()

public:
	class AServerLobbyPawn* pawn;

	virtual void BeginPlay()override;
};
