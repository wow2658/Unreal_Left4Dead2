// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "L4GDChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class R4GD_API UL4GDChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UScrollBox* sb_TextList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UEditableText* eatext_Chat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget), Category=MySettings)
	class UHorizontalBox* hb_InputText;

	UFUNCTION(BlueprintCallable)
	void ChatUP(const FText& Text, ETextCommit::Type CommitMethod);

	//UFUNCTION(BlueprintCallable)
	//void ChatUP(FString& chatString);
};
