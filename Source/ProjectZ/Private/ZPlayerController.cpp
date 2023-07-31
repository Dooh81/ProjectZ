// Fill out your copyright notice in the Description page of Project Settings.


#include "ZPlayerController.h"
#include "EngineUtils.h"
#include "ZSplineMovementComponent.h"
#include "ZBicycleCharacter.h"
#include "ZPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AZPlayerController::AZPlayerController()
{
	
}

void AZPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AZBicycleCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
}


void AZPlayerController::ServerSendDisplayUserData_Implementation(FDisplayUserData Data)
{
	AZPlayerState* ZPlayerState = Cast<AZPlayerState>(PlayerState);
	if(ZPlayerState)
	{
		ZPlayerState->SetDisplayUserData(Data);
	}
}

bool AZPlayerController::ServerSendDisplayUserData_Validate(FDisplayUserData Data)
{
	return true;
}

void AZPlayerController::ChangeSpline(FName Name)
{
	TArray<class AActor*> Splines = MyCharacter->GetSplineMovementComponent()->splineActors;
	
	for (int i = 0; i < Splines.Num(); i++)
	{
		TArray <FName> tempTags = Splines[i]->Tags;
		
		for(int j = 0; j <tempTags.Num(); j++)
		{
			if (tempTags[j] == Name)
			{
				MyCharacter->SetCurrentSplineComponent(Splines[i]);
			}

			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("Not Found Collected Spline!!"));
			}
		}
		
	}

	
}


