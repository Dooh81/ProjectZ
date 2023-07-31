// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MapCourceWidget.h"

#include "TextReaderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "ZGameInstance.h"

UMapCourceWidget::UMapCourceWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	//텍스트 리더로 txt파일 읽어오기 공통부분
	UTextReaderComponent* TextReader = CreateDefaultSubobject<UTextReaderComponent>(TEXT("TextReaderComp"));
	LocalServerURL = TextReader->ReadFile("Urls/LocalServerURL.txt");
}

void UMapCourceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate R1Delegate;
	R1Delegate.BindUFunction(this, "OnClickedR1");
	Button_R1->OnClicked.Add(R1Delegate);

	FScriptDelegate R2Delegate;
	R2Delegate.BindUFunction(this, "OnClickedR2");
	Button_R2->OnClicked.Add(R2Delegate);

	FScriptDelegate R3Delegate;
	R3Delegate.BindUFunction(this, "OnClickedR3");
	Button_R3->OnClicked.Add(R3Delegate);

	FScriptDelegate StartDelegate;
	StartDelegate.BindUFunction(this, "OnClickedStart");
	Button_Start->OnClicked.Add(StartDelegate);
}

void UMapCourceWidget::OnClickedR1()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clicked R1"));
	StartingPoint = "R1";
}

void UMapCourceWidget::OnClickedR2()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clicked R2"));
	StartingPoint = "R2";
}

void UMapCourceWidget::OnClickedR3()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Clicked R3"));
	StartingPoint = "R3";
}

void UMapCourceWidget::OnClickedStart()
{

	//local
	//FString Level = LocalServerURL;
	FString Level = "CityPark";
	FString Options = "Route=";
	FString AddPoint = StartingPoint;
	Options+=AddPoint;
	
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance)
	{
		ZGameInstance->StartingPoint = FName(*StartingPoint);
		ZGameInstance->ShowLoadingWidgetAfterLoadLevel(Level, Options);
	}
}

void UMapCourceWidget::OnClickedOpenLevel(ELevelName Name)
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance != nullptr)
	{
		ZGameInstance->LoadingAfterLoadLevel(Name);
	}
}
