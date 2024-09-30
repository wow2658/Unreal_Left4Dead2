// Fill out your copyright notice in the Description page of Project Settings.


#include "R4GDGameModeBase.h"
#include "R4GD.h"

AR4GDGameModeBase::AR4GDGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Dev/LGT/Hank_BP/BP_Hank.BP_Hank_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}
