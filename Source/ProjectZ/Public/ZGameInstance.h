// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons/Commons.h"
#include "Engine/GameInstance.h"
#include "ZGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UZGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

protected:
	UPROPERTY(EditAnywhere, Category="Widget Classes")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Loading")
	UUserWidget* LoadingWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName CurrentLevel;

	//SpawnLocation 및 current spline 용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName StartingPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AZBicycleCharacter* PlayerCharacter;

	UFUNCTION()
	void CreateLoadingWidget();

	UFUNCTION(BlueprintCallable)
	void ShowLoadingWidgetAfterLoadLevel(FString Level, FString Options);

	UFUNCTION()
	void LoadingAfterLoadLevel(ELevelName Level);

	UFUNCTION()
	void SetPlayerAvatarInfo(FPlayerAvatarInfo info);

	UFUNCTION()
	FPlayerAvatarInfo GetAvatarInfo();

	UFUNCTION()
	void SetInventoryItems(TArray<int32> Items);

	UFUNCTION()
	TArray<int32> GetInventoryItems();

	UFUNCTION()
	void SetEquipmentInfo(FPlayerEquipmentInfo Info);


	UPROPERTY()
	FBicycleCalcData BicycleCalcData;

private:
	UPROPERTY()
	FPlayerEquipmentInfo PlayerEquipmentInfo;

	UPROPERTY()
	FPlayerAvatarInfo PlayerAvatarInfo;

	UPROPERTY()
	TArray<int32> InventoryItems;

};
