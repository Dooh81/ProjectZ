// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons/Commons.h"
#include "GameFramework/PlayerController.h"
#include "ZPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZPlayerController : public APlayerController
{
	GENERATED_BODY()

	AZPlayerController();

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AZBicycleCharacter* MyCharacter;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendDisplayUserData(FDisplayUserData Data);

	UFUNCTION(BlueprintCallable)
	void ChangeSpline(FName Name);
	
	FVector StartLocation;
};
