// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "L4GDLobbyMenuWidget.generated.h"

UCLASS()
class R4GD_API UL4GDLobbyMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UL4GDLobbyWidget* owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Campaign;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_TransformMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_CampaignRealism;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FightMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FightModeRealism;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_SurvivalPlay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_FightSurvival;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Skavinge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UButton* btn_NextPage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	TArray<UButton*> List;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	TArray<UTextBlock*> TextBlocks;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UTextBlock* Title;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UTextBlock* TextLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	int32 CurrentIndex;

public:
	UFUNCTION()
	void OnClickedbtn_CamPaign();

	UFUNCTION()
	void OnClickedbtn_Left();

	UFUNCTION()
	void OnClickedbtn_Right();

	UFUNCTION()
	void OnClickedbtn_NextPage();

	UFUNCTION(BlueprintCallable)
	void ButtonUpdateText();

protected:
	UFUNCTION()
	void NativeConstruct();
	
};