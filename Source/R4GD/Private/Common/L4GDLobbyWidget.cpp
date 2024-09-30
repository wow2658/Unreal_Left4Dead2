// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDLobbyWidget.h"
#include "Components/WidgetSwitcher.h"
#include "L4GDLobbyMenuWidget.h"
#include "L4GDLobbySessionWidget.h"
#include "L4GDLobbyRoomWidget.h"
#include "R4GDGameInstance.h"
#include "ServerLobbyController.h"

void UL4GDLobbyWidget::NativeConstruct()
{
	UR4GDGameInstance* gameInstance = GetGameInstance<UR4GDGameInstance>();
	bp_MenuWidget->owner = this;
	bp_SessionWidget->owner = this;
	bp_RoomWidget->owner = this;

	if (gameInstance)
	{
		if (gameInstance->currentNamedSession)
		{
			ws_ChangeWidget->SetActiveWidgetIndex(2);
		}
	}
	//ShowCursor(true);
	//menuWidget = CreateWidget<UL4GDLobbyMenuWidget>(ws_ChangeWidget, menuWidgetBP);
	//sessionWidget = CreateWidget<UL4GDLobbySessionWidget>(ws_ChangeWidget, sessionWidgetBP);
	//roomWidget = CreateWidget<UL4GDLobbyRoomWidget>(ws_ChangeWidget, roomWidgetBP);
}