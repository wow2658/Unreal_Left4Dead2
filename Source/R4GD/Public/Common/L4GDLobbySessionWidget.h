// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "L4GDLobbySessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UL4GDLobbySessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class UR4GDGameInstance* gameInstance;

	UPROPERTY()
	class UWidgetSwitcher* parent;

	UPROPERTY()
	class UL4GDLobbyWidget* owner;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UScrollBox* sb_RoomList;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_CreateSession;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_Back;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_Join;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UCircularThrobber* ct_Loading;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_PlayerCount;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)

	class UVerticalBox* vb_RoomInfo;
	UPROPERTY(EditDefaultsOnly, Category = MySettings, BlueprintReadWrite)
	TSubclassOf<class UL4GDSessionInfoWidget> sessionInfoWidget;

	float loadTimer;

	int selectSessionIndex = -4;
private:
	UPROPERTY()
	class UL4GDSessionInfoWidget* sessionSlot;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnClickCreateSession();

	UFUNCTION()
	void OnClickBack();

	UFUNCTION()
	void OnClickJoinSession();

	UFUNCTION()
	void SearchFinish();

	UFUNCTION()
	void CreateFinish();

	UFUNCTION()
	void AddRoomSlot(FSessionSlotInfo slotinfo);

	void UpdateRoomInfo(FSessionSlotInfo slotinfo);
};
