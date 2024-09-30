// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDChatWidget.h"
#include <UMG/Public/Components/EditableText.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/ScrollBox.h>

void UL4GDChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UL4GDChatWidget::ChatUP(const FText& Text, ETextCommit::Type CommitMethod)
{
	UTextBlock* TextWidget = NewObject<UTextBlock>(this);
	if(TextWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Text.ToString());
		TextWidget->SetText(Text);
		TextWidget->SetAutoWrapText(true);
		eatext_Chat->SetText(FText());
		sb_TextList->AddChild(TextWidget);
		sb_TextList->ScrollToEnd();
	}
}
//void UL4GDChatWidget::ChatUP(FString& chatString)
//{
//
//}
//