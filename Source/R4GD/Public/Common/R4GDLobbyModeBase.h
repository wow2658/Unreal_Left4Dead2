// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "R4GDLobbyModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoginSuccessed);
/**
 * 
 */
UCLASS()
class R4GD_API AR4GDLobbyModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	FLoginSuccessed OnLoginSuccessed;
	
	UFUNCTION()
	virtual void OnPostLogin(AController* NewPlayer)override;

	virtual void Logout(AController* ExitingPlayer)override;
	
	virtual void BeginPlay() override;
	//TArray<class AServerLobbyController*> connetedPlayers;
};
