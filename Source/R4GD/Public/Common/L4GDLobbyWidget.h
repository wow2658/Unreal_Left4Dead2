// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "L4GDLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UL4GDLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	virtual void NativeConstruct() override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<class UL4GDLobbyMenuWidget> menuWidgetBP;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<class UL4GDLobbySessionWidget> sessionWidgetBP;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<class UL4GDLobbyRoomWidget> roomWidgetBP;
	//
	//class UL4GDLobbyMenuWidget* menuWidget;
	//
	//class UL4GDLobbySessionWidget* sessionWidget;
	//
	//class UL4GDLobbyRoomWidget* roomWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UL4GDLobbyMenuWidget* bp_MenuWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UL4GDLobbySessionWidget* bp_SessionWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UL4GDLobbyRoomWidget* bp_RoomWidget;
	//void ChangeWidget(int index);
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = MySettings)
	class UWidgetSwitcher* ws_ChangeWidget;
};
