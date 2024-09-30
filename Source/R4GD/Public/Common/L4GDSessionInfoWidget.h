// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "R4GDGameInstance.h"
#include "L4GDSessionInfoWidget.generated.h"


/**
 * 
 */
UCLASS()
class R4GD_API UL4GDSessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ClickFull();

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_Full;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerCount;

	class UL4GDLobbySessionWidget* owner;

	struct FSessionSlotInfo sessionInfo;

	int32 sessionIndex;

	
};
