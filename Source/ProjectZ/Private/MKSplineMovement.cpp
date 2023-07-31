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

	//�ʱ� ĳ���� ��ġ ����
	//CurrentPlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();


	//���忡 ��ġ�� ���ö��ξ��� �����̳ʿ� �߰� 
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

//�÷��̾� ��ġ���� ����� ���ö��� ����Ʈ Ž��
void UMKSplineMovement::FindSplinePoint()
{

	if (IsValid(currentSpline))
	{
		splinePointLoc = currentSpline->SplineComponent->GetLocationAtDistanceAlongSpline(moveDistance, ESplineCoordinateSpace::World);	 
		splinePointRot = currentSpline->SplineComponent->FindRotationClosestToWorldLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), ESplineCoordinateSpace::World);
		Angles.Add(currentSpline->SplineComponent->FindRotationClosestToWorldLocation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), ESplineCoordinateSpace::Local).Yaw);
	}
}


//�÷��̾��� ���� �̵��Ÿ� ���
void UMKSplineMovement::CalcDistance(float time)
{
	//������ �̵��Ÿ� �ʱ�ȭ
	if (moveDistance >= currentSpline->SplineComponent->GetSplineLength())
	{
		moveDistance = 0;
	}

	//ĳ������ �̵��Ÿ� ���
	if (currentSpline)
	{
		if (moveDistance <= currentSpline->SplineComponent->GetSplineLength() && CurrentPlayerLocation != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation())
		{
			moveDistance = moveDistance + (time * UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetCharacterMovement()->GetMaxSpeed());

			CurrentPlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(); //ĳ���� ��ġ ������Ʈ

		}
	}

}


//���ö��� ����Ʈ�� ȸ���� ����
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



