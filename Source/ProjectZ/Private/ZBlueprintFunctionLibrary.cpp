// Fill out your copyright notice in the Description page of Project Settings.


#include "ZBlueprintFunctionLibrary.h"
#include "ZLoadingScreen/ZLoadingScreen.h"

void UZBlueprintFunctionLibrary::PlayerLoadingScreen(UUserWidget* Widget, bool bPlayUntilStopped, float PlayTime)
{
	IZLoadingScreenModule& LoadingScreenModule = IZLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(Widget, bPlayUntilStopped, PlayTime);
}

void UZBlueprintFunctionLibrary::StopLoadingScreen()
{
	IZLoadingScreenModule& LoadingScreenModule = IZLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

bool UZBlueprintFunctionLibrary::IsInEditor()
{
	return GIsEditor;
}
