// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomerAnimInstance.h"
#include "Net/UnrealNetwork.h"


void UBoomerAnimInstance::OnHandAttackEvent()
{
	HandAttackMode = false;
}

void UBoomerAnimInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UBoomerAnimInstance, boomerState);
    DOREPLIFETIME(UBoomerAnimInstance, HandAttackMode);
}
