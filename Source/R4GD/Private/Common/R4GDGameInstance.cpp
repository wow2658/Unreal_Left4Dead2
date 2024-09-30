// Fill out your copyright notice in the Description page of Project Settings.


#include "R4GDGameInstance.h"
#include "OnlineSubsystemSteam.h"
#include "OnlineSessionSettings.h"
#include "ServerLobbyController.h"

UR4GDGameInstance::UR4GDGameInstance()
{
	
	playerName = TEXT("ZombieGod");

}

void UR4GDGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* subsys = IOnlineSubsystem::Get())
	{
		//API���ٿ� �ν��Ͻ� ��������
		sessionInterface = subsys->GetSessionInterface();

		//���� �̺�Ʈ�� �Լ� ���ε�
		if (sessionInterface)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UR4GDGameInstance::OnCreateSelfSession);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UR4GDGameInstance::OnFindOtherSessions);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UR4GDGameInstance::OnJoinSelectedSession);
			sessionInterface->OnUpdateSessionCompleteDelegates.AddUObject(this, &UR4GDGameInstance::OnUpdateSession);
			//sessionInterface->OnRegisterPlayersCompleteDelegates.AddUObject(this, &UR4GDGameInstance::OnRegisterPlayersComplete);
		}
		IdentityInterface = subsys->GetIdentityInterface();
		if (IdentityInterface.IsValid())
		{
			playerNetId = IdentityInterface->GetUniquePlayerId(0); // 0�� ���� �÷��̾�
			if (playerNetId.IsValid())
			{
				FString DisplayName = IdentityInterface->GetPlayerNickname(0);
				UE_LOG(LogTemp, Warning, TEXT("Steam Display Name: %s"), *DisplayName);
				playerName = DisplayName;// + FString::Printf(TEXT("%f"), GetWorld()->DeltaTimeSeconds);
			}
		}
	}
}

bool UR4GDGameInstance::CreateSelfSession()
{
	FOnlineSessionSettings settings;

	//Lan, ��Ը� ����
	settings.bIsDedicated = false;
	settings.bIsLANMatch = !(IOnlineSubsystem::Get()->GetSubsystemName() == FName("Steam"));
	//�˻� ������ ������ ����
	settings.bShouldAdvertise = true;
	//���� ���
	settings.bUsesPresence = true;
	//�߰� ����
	settings.bAllowJoinInProgress = true;
	settings.bAllowJoinViaPresence = true;

	settings.NumPublicConnections = 4;
	settings.Set(FName("HOST_NAME"), playerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	settings.Set(FName(TEXT("SecretID")), FString(TEXT("LEFT4DEAD_Room")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	bool isSuccess = false;
	for(int index = 0 ; index < 20 ; index++)
	{
		FString RoomName = FString::Printf(TEXT("LEFT4DEAD_Room_%d"), index);
		settings.Set(FName("ROOM_NAME"), RoomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		isSuccess = sessionInterface->CreateSession(0, FName(RoomName), settings);
		if (isSuccess)
		{
			currentSessionName = FName(RoomName);
			currentNamedSession = sessionInterface->GetNamedSession(currentSessionName);
			OnCreateSessionCompleted.Broadcast();
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Create Result : %s"), isSuccess ? *FString("Suc") : *FString("Fal"));
	return isSuccess;
}

void UR4GDGameInstance::OnCreateSelfSession(FName sessionName, bool bIsSuccessful)
{
	if (!bIsSuccessful) return;

	bool result = GetWorld()->ServerTravel("/Game/Dev/KJS/ServerLobbyMap?Listen", true);
	UE_LOG(LogTemp, Warning, TEXT("Tarvel Result : %s"), result ? *FString("TravelSuccessed") : *FString("TravelFail"));
}

void UR4GDGameInstance::FindOtherSession()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());


	// 1. lan�˻����� dedicated�˻����� �����Ѵ�.
	UE_LOG(LogTemp, Warning, TEXT("USE SUBSYS : %s"), *IOnlineSubsystem::Get()->GetSubsystemName().ToString());

	sessionSearch->bIsLanQuery = !(IOnlineSubsystem::Get()->GetSubsystemName() == FName("Steam"));
	
	// 2. ���� ����(query) �����Ѵ�.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 3. ���� �˻����� �����Ѵ�.
	sessionSearch->MaxSearchResults = 10;

	sessionSearch->QuerySettings.Set(FName(TEXT("SecretID")), FString(TEXT("LEFT4DEAD_Room")), EOnlineComparisonOp::Equals);

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());

	//OnSearchFinish.Broadcast();
}

void UR4GDGameInstance::OnFindOtherSessions(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Find Sessions : %s"), bWasSuccessful ? *FString("Suc") : *FString("Fal"));
	OnSearchFinish.Broadcast();
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;


		UE_LOG(LogTemp, Warning, TEXT("Find Sessions : %d"), searchResults.Num());

		for (int i = 0; i < searchResults.Num(); i++)
		{
			FOnlineSessionSearchResult result = searchResults[i];
			FString roomName;
			result.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			if (!roomName.Contains("LEFT4DEAD_Room_")) continue;
			FString hostName;
			result.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
			int32 openNumber = result.Session.NumOpenPublicConnections;
			int32 maxNumber = result.Session.SessionSettings.NumPublicConnections;
			int32 pingSpeed = result.PingInMs;

			UE_LOG(LogTemp, Warning, TEXT("RoomName : %s, HostName : %s, Number : %d/%d, PingSpeed : %d"), *roomName, *hostName, openNumber, maxNumber, pingSpeed);
			
			FSessionSlotInfo slotInfo;
			slotInfo.Set(roomName, hostName, FString::Printf(TEXT("%d/%d"), openNumber, maxNumber), pingSpeed, i);

			OnSearchInfoCompleted.Broadcast(slotInfo);
		}
	}
}

void UR4GDGameInstance::JoinSelectedSession(int32 index)
{
	FString sessionName = TEXT("None");
	sessionSearch->SearchResults[index].Session.SessionSettings.Get(FName("ROOM_NAME"), sessionName);
	sessionInterface->JoinSession(0, FName(sessionName), sessionSearch->SearchResults[index]);
}

void UR4GDGameInstance::OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), result == EOnJoinSessionCompleteResult::Success ? *FString(TEXT("Success")) : *FString(TEXT("Failed")));

	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
	{
		UE_LOG(LogTemp, Warning, TEXT("Successed : %s"), *sessionName.ToString());
		currentSessionName = sessionName;
		currentNamedSession = sessionInterface->GetNamedSession(currentSessionName);
		APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
		if (playerCon != nullptr)
		{
			//��oin�� ���� ȣ��Ʈ�� ServerTravel�ּҸ� �޾ƿ´�
			FString url;
			sessionInterface->GetResolvedConnectString(sessionName, url);
			//�ּҸ� �޾Ҵٸ�, �� �ּҿ� ���� ���� �̵��Ѵ�.
			if (!url.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Connection URL : %s"), *url);
				//int number = JoinNamedSession->NumOpenPublicConnections - JoinNamedSession->SessionSettings.NumPublicConnections;

					//	FName PlayerName;
					//for (FUniqueNetIdRef playerinfo : JoinNamedSession->RegisteredPlayers)
					//{
					//	FUniqueNetIdPtr playerinfoptr = playerinfo.ToSharedPtr();
					//	if (playerinfoptr.IsValid())
					//	{
					//		FString DisplayName = IdentityInterface->GetPlayerNickname(*playerinfoptr);
					//		UE_LOG(LogTemp, Warning, TEXT("Steam Display Name: %s"), *DisplayName);
					//		playerName = DisplayName;
					//	}
					//}
				playerCon->ClientTravel(url, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("Session Is Full"));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Session Does Not Exist");
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Could Not RetrieveAddress");
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Already In Session");
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("%s"), "UnknownError");
		break;
	default:
		break;
	}
}

void UR4GDGameInstance::OnUpdateSession(FName sessionName, bool bIsUpdate)
{
	if (sessionName == currentSessionName)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), "Update");

		OnUpdateOurSession.Broadcast();
	}
}

//void UR4GDGameInstance::OnRegisterPlayersComplete(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccessful)
//{
//	// ���ε��� ��������Ʈ�� ȣ��� �� ����� �ڵ�
//}