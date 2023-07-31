// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

	AMainMenuHUD();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MapCourseWidget;

public:
	virtual void BeginPlay() override;
	void ShowLobbies();

protected:
	UPROPERTY(EditAnywhere, Category="Widget Classes")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category="Widget Classes")
	TSubclassOf<UUserWidget> LobbiesWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget Classes")
	TSubclassOf<UUserWidget> MapCourseWidgetClass;

	UUserWidget* CurrentWidget;

public:
	UFUNCTION(BlueprintCallable)
	void ShowWidget(UUserWidget* WidgetClass);
};
