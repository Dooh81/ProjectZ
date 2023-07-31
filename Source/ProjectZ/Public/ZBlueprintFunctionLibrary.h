
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZBlueprintFunctionLibrary.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Loading)
	static void PlayerLoadingScreen(UUserWidget* Widget, bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable, Category=Loading)
	static void StopLoadingScreen();
	
	UFUNCTION(BlueprintPure, Category=Loading)
	static bool IsInEditor();
	
};
