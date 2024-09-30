// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "R4GDGameInstance.generated.h"

 USTRUCT(BlueprintType)
struct FSessionSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FString roomName = TEXT("");
	FString hostName = TEXT("");
	FString playerCount = TEXT("");
	int32 pingSpeed = 0;
	int32 sessionIndex = 0;
	
	FORCEINLINE void Set(FString rName, FString hName, FString pCount, int32 pSpeed, int32 index){ roomName = rName, hostName = hName, playerCount = pCount, pingSpeed = pSpeed, sessionIndex = index;}
};
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSessionSignature, FSessionSlotInfo, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSearchFinishSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateSessionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSessionCompletedSignature);

UCLASS()
class R4GD_API UR4GDGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UR4GDGameInstance();

	UPROPERTY()
	FString playerName;

	UPROPERTY()
	FName currentSessionName;

	class FNamedOnlineSession* currentNamedSession;
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	IOnlineIdentityPtr IdentityInterface;
	TSharedPtr<const FUniqueNetId> playerNetId;
	FSearchSessionSignature OnSearchInfoCompleted;
	FSearchFinishSignature OnSearchFinish;
	FUpdateSessionSignature OnUpdateOurSession;
	FOnCreateSessionCompletedSignature OnCreateSessionCompleted;
public:
	UFUNCTION()
	bool CreateSelfSession();
	UFUNCTION()
	void OnCreateSelfSession(FName sessionName, bool bIsSuccessful);
	UFUNCTION()
	void FindOtherSession();
	UFUNCTION()
	void OnFindOtherSessions(bool bWasSuccessful);
	UFUNCTION()
	void JoinSelectedSession(int32 index);

	void OnJoinSelectedSession(FName sessionName, enum EOnJoinSessionCompleteResult::Type result);
	UFUNCTION()
	void OnUpdateSession(FName sessionName, bool bIsUpdate);
	//UFUNCTION()
	//void OnRegisterPlayersComplete(FName SessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccessful);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> WidgetClass;
public:
	virtual void Init() override;
};
