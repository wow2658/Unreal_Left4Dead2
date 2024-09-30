

// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDLobbySessionWidget.h"
#include "R4GDGameInstance.h"
#include "Components/Button.h"
#include "L4GDLobbyWidget.h"
#include "Components/WidgetSwitcher.h"
#include "L4GDSessionInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"
#include "Components/VerticalBox.h"
#include "L4GDLobbyRoomWidget.h"
#include "ServerLobbyController.h"

void UL4GDLobbySessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = GetGameInstance<UR4GDGameInstance>();
	parent = Cast<UWidgetSwitcher>(GetParent());
	if (gameInstance)
	{
		gameInstance->OnCreateSessionCompleted.AddDynamic(this, &UL4GDLobbySessionWidget::CreateFinish);
		gameInstance->OnSearchInfoCompleted.AddDynamic(this, &UL4GDLobbySessionWidget::AddRoomSlot);
		gameInstance->OnSearchFinish.AddDynamic(this, &UL4GDLobbySessionWidget::SearchFinish);
	}
	btn_CreateSession->OnClicked.AddDynamic(this, &UL4GDLobbySessionWidget::OnClickCreateSession);
	btn_Back->OnClicked.AddDynamic(this, &UL4GDLobbySessionWidget::OnClickBack);
	btn_Join->OnClicked.AddDynamic(this, &UL4GDLobbySessionWidget::OnClickJoinSession);

}

void UL4GDLobbySessionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ct_Loading->GetVisibility() == ESlateVisibility::Hidden)
			loadTimer -= InDeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), loadTimer)
	if (loadTimer < 0)
	{
		gameInstance->FindOtherSession();
		ct_Loading->SetVisibility(ESlateVisibility::Visible);
		loadTimer = 20;
	}
}

void UL4GDLobbySessionWidget::OnClickCreateSession()
{
	if (parent && gameInstance)
	{
		gameInstance->CreateSelfSession();
	}
}
void UL4GDLobbySessionWidget::OnClickBack()
{
	if (parent)
	{
		parent->SetActiveWidgetIndex(0);
		UE_LOG(LogTemp, Warning, TEXT("Create Result"));
	}
}

void UL4GDLobbySessionWidget::OnClickJoinSession()
{
	gameInstance->JoinSelectedSession(selectSessionIndex);
}

void UL4GDLobbySessionWidget::SearchFinish()
{
	sb_RoomList->ClearChildren();
	ct_Loading->SetVisibility(ESlateVisibility::Hidden);
}

void UL4GDLobbySessionWidget::CreateFinish()
{
	parent->SetActiveWidgetIndex(2);
}

void UL4GDLobbySessionWidget::AddRoomSlot(FSessionSlotInfo slotinfo)
{
	//�������� ����.
	UE_LOG(LogTemp, Warning, TEXT("%s"), *slotinfo.roomName);

	sessionSlot = CreateWidget<UL4GDSessionInfoWidget>(GetWorld(), sessionInfoWidget);
	if (sessionInfoWidget)
	UE_LOG(LogTemp, Warning, TEXT("Create Result %s"), *sessionInfoWidget->GetName());
	if (sessionSlot != nullptr)
	{
		//sessionSlot->text_PlayerName->SetText(FText::FromString(slotinfo.roomName));
		sessionSlot->text_PlayerName->SetText(FText::FromString(slotinfo.hostName));
		sessionSlot->text_PlayerCount->SetText(FText::FromString(slotinfo.playerCount));
		//sessionSlot->text_pingSpeed->SetText(FText::AsNumber(slotinfo.pingSpeed));
		sessionSlot->sessionIndex = slotinfo.sessionIndex;
		sessionSlot->sessionInfo = slotinfo;
		sessionSlot->owner = this;
		sb_RoomList->AddChild(sessionSlot);
	}


	//������ ������ ��ũ�� �ڽ� �ڽ����� �߰��Ѵ�.

	//������ ���� ������ ��ũ�� �ڽ��� �ڽ����� �߰��Ѵ�.
}

void UL4GDLobbySessionWidget::UpdateRoomInfo(FSessionSlotInfo slotinfo)
{
	text_PlayerName->SetText(FText::FromString(slotinfo.hostName));
	text_PlayerCount->SetText(FText::FromString(slotinfo.playerCount));
	vb_RoomInfo->SetVisibility(ESlateVisibility::Visible);
}