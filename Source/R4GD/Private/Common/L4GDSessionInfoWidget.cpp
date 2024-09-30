// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDSessionInfoWidget.h"
#include "Components/Button.h"
#include "L4GDLobbySessionWidget.h"

void UL4GDSessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Full->OnClicked.AddDynamic(this, &UL4GDSessionInfoWidget::ClickFull);
}
void UL4GDSessionInfoWidget::ClickFull()
{
	//UE_LOG(LogTemp, Warning, TEXT("123213"));
	owner->selectSessionIndex = sessionIndex;
	owner->UpdateRoomInfo(sessionInfo);
}