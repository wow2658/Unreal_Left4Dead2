// Fill out your copyright notice in the Description page of Project Settings.


#include "L4GDLobbyMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "L4GDLobbyWidget.h"



void UL4GDLobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	List.Add(btn_Campaign);
	List.Add(btn_TransformMode);
	List.Add(btn_CampaignRealism);
	List.Add(btn_FightMode);
	List.Add(btn_FightModeRealism);
	List.Add(btn_SurvivalPlay);
	List.Add(btn_FightSurvival);
	List.Add(btn_Skavinge);

	btn_Left->OnClicked.AddDynamic(this,&UL4GDLobbyMenuWidget::OnClickedbtn_Left);
	btn_Right->OnClicked.AddDynamic(this, &UL4GDLobbyMenuWidget::OnClickedbtn_Right);
	btn_Campaign->OnClicked.AddDynamic(this, &UL4GDLobbyMenuWidget::OnClickedbtn_CamPaign);
	btn_NextPage->OnClicked.AddDynamic(this, &UL4GDLobbyMenuWidget::OnClickedbtn_NextPage);
	ButtonUpdateText();
}

void UL4GDLobbyMenuWidget::OnClickedbtn_CamPaign()
{

}

void UL4GDLobbyMenuWidget::OnClickedbtn_Left()
{
	ButtonUpdateText();
}

void UL4GDLobbyMenuWidget::OnClickedbtn_Right()
{
	ButtonUpdateText();
}

void UL4GDLobbyMenuWidget::OnClickedbtn_NextPage()
{
	if (owner)
	{
		owner->ws_ChangeWidget->SetActiveWidgetIndex(1);
	}
}

void UL4GDLobbyMenuWidget::ButtonUpdateText()
{
	UButton* Button = List[1];
	if (Button)
	{
		if (Button == btn_Campaign)
		{
			Title->SetText(FText::FromString(TEXT("캠페인 플레이")));
			TextLine->SetText(FText::FromString(TEXT("협동 캠페인을 온라인으로 플레이합니다.")));
		}
		else if (Button == btn_TransformMode)
		{
			Title->SetText(FText::FromString(TEXT("변형 모드")));
			TextLine->SetText(FText::FromString(TEXT("지상 최후의 생존자 - 단 한 명의 생존자로서, 당신은 감염자들 사이에서 전설로 통할 것입니다.")));
		}
		else if (Button == btn_CampaignRealism)
		{
			Title->SetText(FText::FromString(TEXT("리얼리즘 플레이")));
			TextLine->SetText(FText::FromString(TEXT("진짜 대재앙에서는 훌륭한 팀워크만이 유일한 희망입니다.")));
		}
		else if (Button == btn_FightMode)
		{
			Title->SetText(FText::FromString(TEXT("대전 모드 플레이")));
			TextLine->SetText(FText::FromString(TEXT("온라인 대전 모드 게임에서 생존자 또는 좀비로 플레이합니다.")));
		}
		else if (Button == btn_FightModeRealism)
		{
			Title->SetText(FText::FromString(TEXT("리얼리즘 대전")));
			TextLine->SetText(FText::FromString(TEXT("리얼리즘 대전 - 생존자들은 서로가 보이지 않지만 좀비들은 생존자들이 보입니다.")));
		}
		else if (Button == btn_SurvivalPlay)
		{
			Title->SetText(FText::FromString(TEXT("서바이벌 플레이")));
			TextLine->SetText(FText::FromString(TEXT("서바이벌 플레이 모드입니다. 최대한 오래 버티십시오.")));
		}
		else if (Button == btn_FightSurvival)
		{
			Title->SetText(FText::FromString(TEXT("대전 서바이벌")));
			TextLine->SetText(FText::FromString(TEXT("대전 서바이벌 - 여러분의 친구와 대항해 살아남을 수 있겠습니까?")));
		}
		else if (Button == btn_Skavinge)
		{
			Title->SetText(FText::FromString(TEXT("스캐빈지 플레이")));
			TextLine->SetText(FText::FromString(TEXT("라운드 기반 경쟁 매치에서 생존자 또는 좀비로 플레이합니다.")));
		}
	}
}