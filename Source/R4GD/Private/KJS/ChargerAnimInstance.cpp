// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargerAnimInstance.h"
#include "Net/UnrealNetwork.h"

void UChargerAnimInstance::OnHandAttackEvent()
{
	HandAttackMode = false;
}

void UChargerAnimInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UChargerAnimInstance, ChargerState);
    DOREPLIFETIME(UChargerAnimInstance, HandAttackMode);
}