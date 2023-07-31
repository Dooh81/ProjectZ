// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons/Commons.h"
#include "GameFramework/PlayerState.h"
#include "ZPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZPlayerState : public APlayerState
{
	GENERATED_BODY()

private:
	UPROPERTY(Replicated)
	FDisplayUserData UserData;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	FDisplayUserData GetDisplayUserData(){return UserData;}

	UFUNCTION()
	void SetDisplayUserData(FDisplayUserData Data){UserData = Data;}
};
