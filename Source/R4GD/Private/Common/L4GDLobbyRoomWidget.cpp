// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDLobbyRoomWidget.h"
#include "L4GDLobbyWidget.h"
#include "R4GDGameInstance.h"
#include "Components/TextBlock.h"
#include "../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "Components/Button.h"
#include "R4GDLobbyModeBase.h"
#include "ServerLobbyController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "ServerLobbyPawn.h"

void UL4GDLobbyRoomWidget::NativeConstruct()
{
	btn_Start->OnClicked.AddDynamic(this, &UL4GDLobbyRoomWidget::GameStart);
	playerNameArray.Add(text_PlayerName01);
	playerNameArray.Add(text_PlayerName02);
	playerNameArray.Add(text_PlayerName03);
	playerNameArray.Add(text_PlayerName04);
	playerImageArray.Add(image_PlayerImage01);
	playerImageArray.Add(image_PlayerImage02);
	playerImageArray.Add(image_PlayerImage03);
	playerImageArray.Add(image_PlayerImage04);
	playerTextureArray.Add(Cast<UTexture2D>(image_PlayerImage01->GetBrush().GetResourceObject()));
	playerTextureArray.Add(Cast<UTexture2D>(image_PlayerImage02->GetBrush().GetResourceObject()));
	playerTextureArray.Add(Cast<UTexture2D>(image_PlayerImage03->GetBrush().GetResourceObject()));
	playerTextureArray.Add(Cast<UTexture2D>(image_PlayerImage04->GetBrush().GetResourceObject()));

}

void UL4GDLobbyRoomWidget::UpdatePlayers(const TArray<FString>& playerNames)
{
	UE_LOG(LogTemp, Warning, TEXT("GetCount : %d"), playerNames.Num());

	for (int i = 0; i < playerNames.Num(); i++)
	{
		playerNameArray[i]->SetText(FText::FromString(playerNames[i]));
		playerImageArray[i]->SetBrushFromTexture(playerTextureArray[i]);
		UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	}
	for (int i = playerNames.Num(); i < 4; i++)
	{
		FString PlayerName = FString::Printf(TEXT("빈 플레이어 자리"));
		playerNameArray[i]->SetText(FText::FromString(PlayerName));
		playerImageArray[i]->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("%dReset"), i);
	}
}

//void UL4GDLobbyRoomWidget::UpdatePlayers(TArray<FPlayerInfo>& playerInfos)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Players : %d"), GetWorld()->GetNumPlayerControllers());
//
//	if (gameInstance->currentNamedSession && GetWorld()->GetFirstPlayerController()->HasAuthority())
//	{
//		//AR4GDLobbyModeBase* gameBase = Cast<AR4GDLobbyModeBase>(GetWorld()->GetAuthGameMode());
//		//if (gameBase)
//		//{
//		//	for (int i = 0; i < gameBase->connetedPlayers.Num(); i++)
//		//	{
//		//		UE_LOG(LogTemp, Warning, TEXT("3333"));
//		//		if (gameBase->connetedPlayers[i] != NULL)
//		//		{
//		//			FString PlayerName = gameBase->connetedPlayers[i]->playerInfo.playerName;
//		//			playerNameArray[i]->SetText(FText::FromString(PlayerName));
//		//		}
//		//		i++;
//		//	}
//		//}
//		for (int i = 0; i < playerInfos.Num(); i++)
//		{
//			//const FUniqueNetIdPtr UniquePlayerId = gameInstance->IdentityInterface->GetUniquePlayerId(i);
//			//FString PlayerName = gameInstance->IdentityInterface->GetPlayerNickname(*UniquePlayerId);
//			playerNameArray[i]->SetText(FText::FromString(playerInfos[i].playerName));
//		}
//		//UE_LOG(LogTemp, Warning, TEXT("%d"), );
//		////FUniqueNetIdPtr playerPtr = gameInstance->IdentityInterface->GetUniquePlayerId(0);
//		////UE_LOG(LogTemp, Warning, TEXT("%s"), *gameInstance->IdentityInterface->GetPlayerNickname(*playerPtr));
//		//
//		//TArray<FUniqueNetIdRef> PlayerIds = gameInstance->i; // �÷��̾� ��� ��������
//		//UE_LOG(LogTemp, Warning, TEXT("PlayerCount %d"), PlayerIds.Num());
//		//
//	}
//
//}

void UL4GDLobbyRoomWidget::GameStart()
{
	FString mapUrl = TEXT("/Game/MainLevel_QA3?Listen");
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	if (playerCon->HasAuthority())
	{
		GetWorld()->ServerTravel(mapUrl, true);
	}
}
