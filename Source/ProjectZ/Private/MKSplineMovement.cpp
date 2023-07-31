// Fill out your copyright notice in the Description page of Project Settings.


#include "MKSplineMovement.h"
#include "ZSplineActor.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SplineComponent.h"
#include "ZBicycleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMKSplineMovement::UMKSplineMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UMKSplineMovement::BeginPlay()
{
	Super::BeginPlay();

	//초기 캐릭터 위치 저장
	//CurrentPlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();


	//월드에 배치된 스플라인액터 컨테이너에 추가 
	for (TActorIterator<AZSplineActor> SplineIter(GetWorld()); SplineIter; ++SplineIter)
	{
		if (SplineIter->GetWorld())
		{
			SplineMap.Add(UKismetSystemLibrary::GetDisplayName(*SplineIter), *SplineIter);

			if (SplineIter->ActorHasTag(FName(TEXT("1"))))
			{
				currentSpline = *SplineIter;
			}
		}

	}

}


// Called every frame
void UMKSplineMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CalcDistance(DeltaTime);
	//FindSplinePoint();

}

//플레이어 위치에서 가까운 스플라인 포인트 탐색
void UMKSplineMovement::FindSplinePoint()
{

	if (IsValid(currentSpline))
	{
		splinePointLoc = currentSpline->SplineComponent->GetLocationAtDistanceAlongSpline(moveDistance, ESplineCoordinateSpace::World);	 
		splinePointRot = currentSpline->SplineComponent->FindRotationClosestToWorldLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), ESplineCoordinateSpace::World);
		Angles.Add(currentSpline->SplineComponent->FindRotationClosestToWorldLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), ESplineCoordinateSpace::Local).Yaw);
	}
}


//플레이어의 현재 이동거리 계산
void UMKSplineMovement::CalcDistance(float time)
{
	//도착시 이동거리 초기화
	if (moveDistance >= currentSpline->SplineComponent->GetSplineLength())
	{
		moveDistance = 0;
	}

	//캐릭터의 이동거리 계산
	if (currentSpline)
	{
		if (moveDistance <= currentSpline->SplineComponent->GetSplineLength() && CurrentPlayerLocation != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation())
		{
			moveDistance = moveDistance + (time * UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetCharacterMovement()->GetMaxSpeed());

			CurrentPlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(); //캐릭터 위치 업데이트

		}
	}

}


//스플라인 포인트별 회전값 보정
void UMKSplineMovement::CalcAngle()
{
	float MaxSteeringAngle = 10;

	if (IsValid(currentSpline))
	{
		if (Angles.IsValidIndex(Angles.Num() - 1))
		{
			if(Angles.Find(Angles.Num() - 1) - Angles.Find(Angles.Num() - 2) >= 0.2 || Angles.Find(Angles.Num() - 1) - Angles.Find(Angles.Num() - 2) <= -0.2)
			{
				CurrentSteeringAngle = FMath::FInterpConstantTo(CurrentSteeringAngle, (Angles.Find(Angles.Num() - 1) - Angles.Find(Angles.Num() - 2) * MaxSteeringAngle), GetWorld()->GetDeltaSeconds(), 50.0);
			}

			else 
			{
				CurrentSteeringAngle = FMath::FInterpConstantTo(CurrentSteeringAngle, 0, GetWorld()->GetDeltaSeconds(), 50.0);
			}
	    }
	}
}


 void UMKSplineMovement::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	 Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMKSplineMovement, moveDistance);
	DOREPLIFETIME(UMKSplineMovement, splinePointLoc);
	DOREPLIFETIME(UMKSplineMovement, currentSpline);
}



