// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"

#include "Blueprint/UserWidget.h"

AMainMenuHUD::AMainMenuHUD()
{
	
}

/**
 * @brief 메인메뉴 Ui 노출
 */
void AMainMenuHUD::BeginPlay()
{

	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
	}

	
	UUserWidget* MainMenuWidget = CreateWidget(GetWorld(), MainMenuWidgetClass);
	if(MainMenuWidget)
	{
		MainMenuWidget->AddToViewport();
	}

	CurrentWidget = MainMenuWidget;

	//위젯 객체 생성
	MapCourseWidget = CreateWidget(GetWorld(), MapCourseWidgetClass);
}

/**
 * @brief current Ui 제거 후 로비ui 노출
 */
void AMainMenuHUD::ShowLobbies()
{
	if(CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();

		UUserWidget* LobbyWidget = CreateWidget(GetWorld(), LobbiesWidgetClass);
		if(LobbyWidget != nullptr)
		{
			LobbyWidget->AddToViewport();
		}

		CurrentWidget = LobbyWidget;
	}
}

void AMainMenuHUD::ShowWidget(UUserWidget* Widget)
{
	if (CurrentWidget != nullptr)
	{
	   CurrentWidget->RemoveFromParent();

	   if (Widget != nullptr)
	   {
		   Widget->AddToViewport();
	   }

	   CurrentWidget = Widget;
    }

}
