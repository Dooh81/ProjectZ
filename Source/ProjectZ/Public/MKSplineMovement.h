// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "MKSplineMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTZ_API UMKSplineMovement : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UMKSplineMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

		
public:
	//월드 스플라인 담는 맵 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Spline)
		TMap<FString, AActor*> SplineMap;

	//현재 스플라인 담는 액터 변수
	UPROPERTY(replicated, VisibleDefaultsOnly, BlueprintReadWrite, Category = Spline)
		class AZSplineActor* currentSpline;

	//캐릭터 속도
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Spline)
		float speed;

	//캐릭터 이동거리
	UPROPERTY(replicated, VisibleDefaultsOnly, BlueprintReadWrite, Category = Spline)
		float moveDistance;

	//스플라인 포인트 위치
	UPROPERTY(replicated, EditDefaultsOnly, BlueprintReadWrite, Category = Spline)
		FVector splinePointLoc;

	//스플라인 포인트 회전값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spline)
		FRotator splinePointRot;

	//초기 캐릭터 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spline)
		FVector CurrentPlayerLocation;

	//스플라인 포인트 Yaw 회전 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spline)
		TArray <float> Angles;

	//스플라인 포인트 최대 회전 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spline)
		float CurrentSteeringAngle;


public:
	//월드 스플라인 정보
	UFUNCTION(BlueprintCallable, Category = Spline)
	void FindSplinePoint();

	//캐릭터 움직인 거리
	UFUNCTION(BlueprintCallable, Category = Spline)
		void CalcDistance(float time);

	//캐릭터 스플라인 무브먼트

	//캐릭터 회전
	UFUNCTION(BlueprintCallable, Category = Spline)
		void CalcAngle();

};
