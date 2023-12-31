// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZLoadingScreen.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

class FZLoadingScreenModule : public IZLoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// Force load for cooker reference
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_MyProject_TransparentLogo.T_MyProject_TransparentLogo") );
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen(UUserWidget *Widget, bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
	}
};

IMPLEMENT_GAME_MODULE(FZLoadingScreenModule, ZLoadingScreen);
