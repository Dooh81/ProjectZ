// Fill out your copyright notice in the Description page of Project Settings.

/*
	정의 : 스플라


*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZSplineActor.generated.h"

UCLASS()
class PROJECTZ_API AZSplineActor : public AActor
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class USplineComponent* SplineComponent;

public:	
	// Sets default values for this actor's properties
	AZSplineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
