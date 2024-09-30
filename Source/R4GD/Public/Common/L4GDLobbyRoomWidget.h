// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "L4GDLobbyRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UL4GDLobbyRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UR4GDGameInstance* gameInstance;

	UPROPERTY()
	class UL4GDLobbyWidget* owner;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_Start;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName01;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName02;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName03;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName04;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* image_PlayerImage01;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* image_PlayerImage02;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* image_PlayerImage03;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* image_PlayerImage04;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UL4GDChatWidget* bp_ChatWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MySettings);
	TArray<class UTextBlock*> playerNameArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MySettings);
	TArray<class UTexture2D*> playerTextureArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MySettings);
	TArray<class UImage*> playerImageArray;
public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdatePlayers(const TArray<FString>& playerNames);

	UFUNCTION()
	void GameStart();
};
