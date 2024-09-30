// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ServerLobbyPawn.generated.h"

UCLASS()
class R4GD_API AServerLobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AServerLobbyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerChatUp(const FText& text, ETextCommit::Type CommitMethod);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void MulticastChatUp(const FText& text, ETextCommit::Type CommitMethod);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUpdatePlayers();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdatePlayers(const TArray<FString>& playerNames);

public:

	class UR4GDGameInstance* gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MySettings)
	class UMediaSoundComponent* mediaSoundComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MySettings)
	class UMediaSource* mediaSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MySettings)
	class UMediaPlayer* mediaPlayer;

	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UL4GDLobbyWidget> lobbyWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = MySettings)
	class UL4GDLobbyWidget* lobbyWidget;

	class AServerLobbyController* controller;
};
