// Fill out your copyright notice in the Description page of Project Settings.


#include "ZGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ZLoadingScreen/ZLoadingScreen.h"

UZGameInstance::UZGameInstance()
{
}

void UZGameInstance::Init()
{

	Super::Init();

	CreateLoadingWidget();
}

void UZGameInstance::Shutdown()
{
	Super::Shutdown();
	
}

void UZGameInstance::CreateLoadingWidget()
{
	LoadingWidget = CreateWidget(GetWorld(), LoadingWidgetClass);
}

void UZGameInstance::ShowLoadingWidgetAfterLoadLevel(FString Level, FString Options)
{
	IZLoadingScreenModule& LoadingScreenModule = IZLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(LoadingWidget, true, 2.0f);

	UGameplayStatics::OpenLevel(GetWorld(), *Level, true, Options);
}

void UZGameInstance::LoadingAfterLoadLevel(ELevelName Level)
{
	const FString LevelName = GetEnumerationToString(Level);

	UE_LOG(LogTemp,Error,TEXT("Load level Name : %s"), *LevelName);
	
	IZLoadingScreenModule& LoadingScreenModule = IZLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(LoadingWidget, true, 2.0f);
	
	UGameplayStatics::OpenLevel(GetWorld(), *LevelName);
}

void UZGameInstance::SetPlayerAvatarInfo(FPlayerAvatarInfo info)
{
	PlayerAvatarInfo = info;
}

FPlayerAvatarInfo UZGameInstance::GetAvatarInfo()
{
	return PlayerAvatarInfo;
}

void UZGameInstance::SetInventoryItems(TArray<int32> Items)
{
	InventoryItems = Items;
}

TArray<int32> UZGameInstance::GetInventoryItems()
{
	return InventoryItems;
}

void UZGameInstance::SetEquipmentInfo(FPlayerEquipmentInfo Info)
{
	PlayerAvatarInfo.PlayerEquipmentInfo = Info;
}

