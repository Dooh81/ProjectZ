// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerState.h"

#include "Net/UnrealNetwork.h"


void AZPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Replicate the PlayerData property to all clients
	DOREPLIFETIME(AZPlayerState, UserData);
}
