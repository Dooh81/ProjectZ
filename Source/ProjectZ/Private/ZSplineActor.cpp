// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSplineActor.h"

#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AZSplineActor::AZSplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AZSplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZSplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

