// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerLobbyPawn.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "L4GDChatWidget.h"
#include "L4GDLobbyWidget.h"
#include "L4GDLobbyRoomWidget.h"
#include <UMG/Public/Components/EditableText.h>
#include <Kismet/GameplayStatics.h>
#include "ServerLobbyController.h"
#include "R4GDGameInstance.h"
#include <GameFramework/PlayerState.h>
#include <MediaAssets/Public/MediaPlayer.h>
#include <MediaAssets/Public/MediaSource.h>
#include <MediaAssets/Public/MediaSoundComponent.h>
#include "R4GDLobbyModeBase.h"

// Sets default values
AServerLobbyPawn::AServerLobbyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AServerLobbyPawn::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<UR4GDGameInstance>(GetWorld()->GetGameInstance());
	//if (GetController())
	//{
	//	controller = Cast<AServerLobbyController>(GetController());
	//	if(controller)
	//	{
	//		//lobbyWidget = CreateWidget<UL4GDLobbyWidget>(GetWorld(), lobbyWidgetClass);
	//		//lobbyWidget->AddToViewport();
	//		//lobbyWidget->bIsFocusable = true;
	//		//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(controller, lobbyWidget);
	//		//controller->bShowMouseCursor = true;
	//		//mediaPlayer->OpenSource(mediaSource);
	//		//mediaSoundComp = Cast<UMediaSoundComponent>(AddComponentByClass(UMediaSoundComponent::StaticClass(), true, GetActorTransform(), true));
	//		//if (mediaSoundComp)
	//		//{
	//		//	mediaSoundComp->SetMediaPlayer(mediaPlayer);
	//		//	mediaSoundComp->SetupAttachment(RootComponent);
	//		//}
	//		//AR4GDLobbyModeBase* gameMode = Cast<AR4GDLobbyModeBase>(GetWorld()->GetAuthGameMode());
	//		//if (gameMode)
	//		//{
	//		//	gameMode->OnLoginSuccessed.AddDynamic(this, &AServerLobbyPawn::ServerUpdatePlayers);
	//		//}
	//	}
	//}
	//if (HasAuthority())
	//{
	//	TArray<FString> playerNames;
	//	for (int i = 0; i < 4; i++)
	//	{
	//		if (GetWorld()->GetNumPlayerControllers() > i)
	//		{
	//
	//			//const FUniqueNetIdPtr UniquePlayerId = gameInstance->IdentityInterface->GetUniquePlayerId(
	//			//	
	//			//);
	//			TObjectPtr<APlayerState> PlayerStatePtr = UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPlayerState<APlayerState>();
	//			if (PlayerStatePtr)
	//			{
	//				FString PlayerName = PlayerStatePtr->GetPlayerName();//gameInstance->IdentityInterface->GetPlayerNickname(*UniquePlayerId);
	//				playerNames.Add(PlayerName);
	//			}
	//			else
	//			{
	//				FString PlayerName = FString::Printf(TEXT("DefualtPlayer_%d"), i);
	//				playerNames.Add(PlayerName);
	//			}
	//		}
	//	}
	//	AServerLobbyPawn* pawn = Cast<AServerLobbyPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//	if(pawn)
	//	{
	//		pawn->MulticastUpdatePlayers(playerNames); // 서버에서 실행되어야 하는 코드
	//		//UE_LOG(LogTemp, Warning, TEXT("DoServerUpdatePlayers"));
	//	}
	//}
	//AServerLobbyPawn* pawn = Cast<AServerLobbyPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//if (pawn)
	//{
	//	pawn->ServerUpdatePlayers();
	//}
	//if (lobbyWidget)
	//	UE_LOG(LogTemp, Warning, TEXT("Create lobbyWidget"));
	//if (lobbyWidget)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("1"));
	//	if (lobbyWidget->bp_RoomWidget)
	//	{	
	//		UE_LOG(LogTemp, Warning, TEXT("2"));
	//		if (lobbyWidget->bp_RoomWidget->bp_ChatWidget)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("3"));
	//			if (lobbyWidget->bp_RoomWidget->bp_ChatWidget->eatext_Chat)
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("4"));
	//				lobbyWidget->bp_RoomWidget->bp_ChatWidget->eatext_Chat->OnTextCommitted.AddDynamic(this, &AServerLobbyPawn::TestFunc);
	//			}
	//		}
	//	}
	//}
}

// Called every frame
void AServerLobbyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AServerLobbyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AServerLobbyPawn::ServerChatUp_Implementation(const FText& text, ETextCommit::Type CommitMethod)
{
	//UE_LOG(LogTemp, Warning, TEXT("SendMessage %s"), *text.ToString());
	MulticastChatUp(text, CommitMethod);
}

void AServerLobbyPawn::ServerUpdatePlayers_Implementation()
{
	TArray<FString> playerNames;
	for (int i = 0; i < 4; i++)
	{
		if (GetWorld()->GetNumPlayerControllers() > i)
		{

			//const FUniqueNetIdPtr UniquePlayerId = gameInstance->IdentityInterface->GetUniquePlayerId(
			//	
			//);
			TObjectPtr<APlayerState> PlayerStatePtr = UGameplayStatics::GetPlayerController(GetWorld(), i)->GetPlayerState<APlayerState>();
			if (PlayerStatePtr)
			{
				FString PlayerName = PlayerStatePtr->GetPlayerName();//gameInstance->IdentityInterface->GetPlayerNickname(*UniquePlayerId);
				playerNames.Add(PlayerName);
			}
			else
			{
				FString PlayerName = FString::Printf(TEXT("DefualtPlayer_%d"), i);
				playerNames.Add(PlayerName);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("DoServerUpdatePlayers"));
	MulticastUpdatePlayers(playerNames);
}

void AServerLobbyPawn::MulticastUpdatePlayers_Implementation(const TArray<FString>& playerNames)
{
	//UE_LOG(LogTemp, Warning, TEXT("DoClientUpdatePlayers"));
	//if(lobbyWidget)
	//lobbyWidget->bp_RoomWidget->UpdatePlayers(playerNames);
}


void AServerLobbyPawn::MulticastChatUp_Implementation(const FText& text, ETextCommit::Type CommitMethod)
{
	////UE_LOG(LogTemp, Warning, TEXT("GetMessage %s"), *text.ToString());
	//if(lobbyWidget)
	//lobbyWidget->bp_RoomWidget->bp_ChatWidget->ChatUP(text, CommitMethod);
}