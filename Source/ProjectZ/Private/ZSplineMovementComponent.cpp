// Fill out your copyright notice in the Description page of Project Settings.


#include "ZSplineMovementComponent.h"

#include "ProjectZCharacter.h"
#include "ZSplineActor.h"
#include "ZBicycleCharacter.h"
#include "ZGameInstance.h"
#include "ZBikeAnimInstance.h"
#include "ZCyclistAnimInstance.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"		
#include "AI/Navigation/AvoidanceManager.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UZSplineMovementComponent::UZSplineMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...




	moveState = ESplineMoveState::FindRoute;
	DirectionFlag = false;


	// movement variables
	avoidMovementRadius = 200;
	movementTargetDistance = 55;
	moveWeight = 1;

	// avoidance variables
	AvoidDirection = EAvoidDirection::Right;
	AvoidLength = 50;
	LineTraceLength = 100;


	// anim variables
	rearWheelDistance = 200;

	bDebug = false;
}

void UZSplineMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UZSplineMovementComponent, moveDirection);
	DOREPLIFETIME(UZSplineMovementComponent, closestKey);
	DOREPLIFETIME(UZSplineMovementComponent, targetLocation);
	DOREPLIFETIME(UZSplineMovementComponent, AvoidDirection);
}


// Called when the game starts
void UZSplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = Cast<AProjectZCharacter>(GetOwner());


	// get all spline actors on the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZSplineActor::StaticClass(), splineActors);

	// movement #1 test direction flag
	DirectionFlag = FMath::RandBool();


	//Anim
	BikeAnim = Cast<AZBicycleCharacter>(player)->BikeAnim;
	CyclistAnim = Cast<AZBicycleCharacter>(player)->CyclistAnim;
}


// Called every frame
void UZSplineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	if (ROLE_Authority == ROLE_Authority)
	{
		// ...
		if (player == nullptr)
			return;

		if(player->GetCharacterMovement()->GetMaxSpeed() <= 0)
		{
			player->GetCharacterMovement()->StopMovementImmediately();
			return;
		}

		switch (moveState)
		{
		case ESplineMoveState::Idle:
			{
				IdleProcess();
			}
			break;

		case ESplineMoveState::FindRoute:
			{
				FindRouteProcess();
			}
			break;

		case ESplineMoveState::OnRoute:
			{
				OnRouteProcess();
			}
			break;
		}

	}

	// for network anim
	auto bike = Cast<AZBicycleCharacter>(player);
	bike->CurrentBikeSpeed = player->GetCharacterMovement()->MaxWalkSpeed;

// 	if (BikeAnim || CyclistAnim)
// 	{
// 		float PlayerMaxWalkSpeed = player->GetCharacterMovement()->MaxWalkSpeed;
// 		bike->CurrentBikeSpeed = PlayerMaxWalkSpeed;
//		auto bike = Cast<AZBicycleCharacter>(player);

// 		if (BikeAnim)
// 			BikeAnim->MaxWalkSpeed = bike->CurrentBikeSpeed;
// 		if (CyclistAnim)
// 			CyclistAnim->MaxWalkSpeed = bike->CurrentBikeSpeed;
// 	}
}

void UZSplineMovementComponent::IdleProcess()
{
}

void UZSplineMovementComponent::FindRouteProcess()
{
	if (currentSpline == nullptr)
	{
		// if (splineActors.Num() <= 0)
		// 	moveState = ESplineMoveState::Idle;
		// else
		// {
		//
		// 	auto tempSplineActor = Cast<AZSplineActor>(splineActors[0]); // temp : get spline index 0
		// 	currentSpline = tempSplineActor->SplineComponent;
		// 	// set current spline to TempSplineActor index 0 of splineActors
		// }
		//
		// return;
	}
	else
		moveState = ESplineMoveState::OnRoute; // change MoveState

}

void UZSplineMovementComponent::OnRouteProcess()
{
	if (currentSpline)
	{
		// FVector_NetQuantize moveDirection;		
		// float closestKey;			
		// FVector_NetQuantize targetLocation;		


//		/* Get all overlapeed actor on Radius : RVO Radius + avoidMovementRadius */
// 		TArray<FHitResult> OutHitPawns;
// 		auto bike = Cast<AZBicycleCharacter>(player);
// 		if(bike == nullptr)
// 			return;
// 		FVector_NetQuantize playerLocation = bike->GetFrameLocation();
// 		//FCollisionShape RVOColSphere = FCollisionShape::MakeSphere( player->GetCharacterMovement()->GetRVOAvoidanceConsiderationRadius() + avoidMovementRadius );
// 		FCollisionShape RVOColSphere = FCollisionShape::MakeSphere( avoidMovementRadius );
// 
// 		if (bDebug)
// 		{
// 			DrawDebugSphere(GetWorld(), playerLocation, RVOColSphere.GetSphereRadius(), 10, FColor::Purple, false, -1, 0, 2);
// 
// 
// 
// 			// debug mesh location
// 			auto tempBike = Cast<AZBicycleCharacter>(player);
// 			FVector CharacterMeshLocation = tempBike->GetFrameLocation();
// 			DrawDebugLine(GetWorld(), player->GetRootComponent()->GetComponentLocation(), CharacterMeshLocation, FColor::Red, false, -1, 0, 10);
// 		}
// 
// 
// 		FCollisionQueryParams colQueryParams;
// 		colQueryParams.AddIgnoredActor(player);
// 		bool isHit = GetWorld()->SweepMultiByChannel(OutHitPawns, playerLocation, playerLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, RVOColSphere, colQueryParams);
// 
// 
// 		/** Array of AProjectZCharacter actors from overlapped actor */
// 		TArray<AProjectZCharacter*> tempZCharacters;
// 		if (isHit)
// 		{
// 			// 
// 			for (auto& Hit : OutHitPawns)
// 			{
// 				auto tempChar = Cast<AProjectZCharacter>(Hit.GetActor());
// 				if (tempChar != nullptr)
// 				{
// 					tempZCharacters.Add(tempChar);
// 				}
// 			}
// 		}



		
		auto bike = Cast<AZBicycleCharacter>(player);
		if (bike != nullptr)
		{
			/**  movement logic : related with Overlapped ZCharacterArray */
			int OverlappedBikeCount = bike->GetOverlappedBikeCount();
			if (OverlappedBikeCount <= 0)	// default movement( no avoidance target in hit array )
			{
				ReturnToCenterServer(bike);
// 				if (bike->GetLocalRole() == ROLE_Authority)
// 					ReturnToCenterMulticast(bike);
			}
			
			AvoidanceProcess(bike);


			SetFramePositionToServer(bike);
// 			if(bike->GetLocalRole() == ROLE_Authority)
// 				SetFramePositionMulticast(bike);
		}

	


		// 		else	// avoid movement 
		// 		{
		// 			 /* 
		// 				1. Get Closest Input key from player location
		// 				2. Get movement Direction At Spline InputKey
		// 			 */
		// 			targetLocation = player->GetActorLocation();
		// 			closestKey = currentSpline->FindInputKeyClosestToWorldLocation(targetLocation);
		// 			moveDirection = currentSpline->GetDirectionAtSplineInputKey(closestKey, ESplineCoordinateSpace::World);
		// 		}

		FollowSplineProcess();


	}
}



void UZSplineMovementComponent::FollowSplineProcess()
{
	/*
	* Movement
		1. Get Closest InputKey from  player Location
		2. targetLocation: Get location at distance(from player location) along spline
		3. get movement direction from TargetLocation
	*/
	closestKey = currentSpline->FindInputKeyClosestToWorldLocation(player->GetActorLocation());
	auto targetexpectdistance = currentSpline->GetDistanceAlongSplineAtSplineInputKey(closestKey) + movementTargetDistance;
	targetLocation = currentSpline->GetLocationAtDistanceAlongSpline(targetexpectdistance, ESplineCoordinateSpace::World);
	//targetLocation = targetLocation + FVector(0, 0, player->GetActorLocation().Z);
	targetLocation.Z = player->GetActorLocation().Z;
	moveDirection = targetLocation - player->GetActorLocation();



// 		/*
// 			anim update
// 				
// 			Find Inputkey Closest To character location - Distance(rear wheel location)
// 			Set anim Rotation
// 		*/
// 		 auto closestKeyRearWheel = currentSpline->FindInputKeyClosestToWorldLocation(player->GetActorLocation() - player->GetActorForwardVector() * rearWheelDistance);
// 		 auto animtargetLocation = currentSpline->GetLocationAtSplineInputKey(closestKeyRearWheel, ESplineCoordinateSpace::World);
// 		 animWheelRotation = (animtargetLocation - player->GetActorLocation()).ToOrientationRotator();
// 		
// 		
// 		
		 // draw debug
	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), targetLocation, 50, 10, FColor::Blue, false, -1, 0, 2);
		DrawDebugLine(GetWorld(), player->GetActorLocation(), currentSpline->GetLocationAtSplineInputKey(closestKey, ESplineCoordinateSpace::World), FColor::Emerald, false, -1, 0, 3);
		//DrawDebugSphere(GetWorld(), player->GetActorLocation(), player->GetCharacterMovement()->GetRVOAvoidanceConsiderationRadius(), 10, FColor::Red, false, -1, 0, 2);
		//DrawDebugLine(GetWorld(), player->GetActorLocation(), animtargetLocation, FColor::Emerald, false, -1, 0, 3);
	}


	// add movement input
	moveDirection.Normalize();


	player->AddMovementInput(moveDirection, moveWeight);

}




void UZSplineMovementComponent::ReturnToCenterServer_Implementation(class AZBicycleCharacter* bike)
{
	ReturnToCenterMulticast(bike);
}

void UZSplineMovementComponent::ReturnToCenterMulticast_Implementation(class AZBicycleCharacter* bike)
{
	if(bike == nullptr) return;
	
	switch (AvoidDirection)
	{
	case EAvoidDirection::Center:
	{

	}
	break;

	case EAvoidDirection::Left:
	{
		auto BikeFrame = bike->GetMeshAttraction();
		if (BikeFrame == nullptr)
			return;

		FVector CurrentFrameLocation = BikeFrame->GetRelativeLocation();
		BikeFrame->SetRelativeLocation(CurrentFrameLocation + FVector(0, AvoidLength, 0));

		CurrentFrameLocation = BikeFrame->GetRelativeLocation();
		if (CurrentFrameLocation.Y >= 0)
		{
			CurrentFrameLocation.Y = 0;
			BikeFrame->SetRelativeLocation(CurrentFrameLocation);

			AvoidDirection = EAvoidDirection::Center;
		}
	}
	break;

	case EAvoidDirection::Right:
	{
			
				
		auto BikeFrame = bike->GetMeshAttraction();
		if (BikeFrame == nullptr)
			return;

		FVector CurrentFrameLocation = BikeFrame->GetRelativeLocation();
		BikeFrame->SetRelativeLocation(CurrentFrameLocation + FVector(0, -AvoidLength, 0));

		CurrentFrameLocation = BikeFrame->GetRelativeLocation();
		if (CurrentFrameLocation.Y <= 0)
		{
			CurrentFrameLocation.Y = 0;
			BikeFrame->SetRelativeLocation(CurrentFrameLocation);

			AvoidDirection = EAvoidDirection::Center;
		}
	}
	break;
	}

}

void UZSplineMovementComponent::AvoidanceProcess(class AZBicycleCharacter* bike)
{
	FVector LineStartPosition =  bike->GetMeshAttractionLocation() + FVector(0.f, 0.f, 0.f);
	FVector LineEndPosition = LineStartPosition + (bike->GetMeshAttraction()->GetUpVector() * LineTraceLength);

	if (bDebug)
		DrawDebugLine(GetWorld(), LineStartPosition, LineEndPosition, FColor::Blue, false, -1.f, 0.f, 5.f);
	
	FCollisionQueryParams TraceParams(NAME_None, false, player);
	//TraceParams.AddIgnoredActor(bike);
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		LineStartPosition,
		LineEndPosition,
		ECC_GameTraceChannel2,
		TraceParams);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		if (HitActor && HitComponent)
		{
			// Get the hit object's name and location
			FString HitObjectName = HitActor->GetName();
			FString HitComponentname = HitComponent->GetName();

			// Print hit object's name and location
			UE_LOG(LogTemp, Warning, TEXT("Hit object: %s, Hit Component: %s"), *HitObjectName, *HitComponentname);
		}

// 		if(player->GetLocalRole() == ROLE_Authority)
		{
			// avoid
			auto FrontBike = Cast<AZBicycleCharacter>(HitActor);
			auto FrontBikeSplineMoveComponent = FrontBike->FindComponentByClass<UZSplineMovementComponent>();
			
			FrontBikeSplineMoveComponent->AvoidToServer(EAvoidDirection::Left);
			AvoidToServer(EAvoidDirection::Right);

// 			FrontBikeSplineMoveComponent->AvoidToMulticast(EAvoidDirection::Left);
// 			AvoidToMulticast(EAvoidDirection::Right);
		}
	}
}

void UZSplineMovementComponent::AvoidToServer_Implementation(EAvoidDirection value)
{
	AvoidToMulticast(value);
}

// void UZSplineMovementComponent::AvoidTo(EAvoidDirection value)
// {
// // 	auto bike = Cast<AZBicycleCharacter>(player);
// // 	if (bike)
// // 	{
// // 		auto BikeFrame = bike->GetMeshAttraction();
// // 		if (BikeFrame)
// // 		{
// // 			float AvoidDirectionLength;
// // 			switch (value)
// // 			{
// // 			case EAvoidDirection::Left:
// // 			{
// // 				AvoidDirectionLength = -AvoidLength;
// // 				AvoidDirection = EAvoidDirection::Left;
// // 			}
// // 			break;
// // 
// // 			case EAvoidDirection::Right:
// // 			{
// // 				AvoidDirectionLength = AvoidLength;
// // 				AvoidDirection = EAvoidDirection::Right;
// // 			}
// // 			break;
// // 			}
// // 
// // 			FVector CurrentFrameLocation = BikeFrame->GetRelativeLocation();
// // 			BikeFrame->SetRelativeLocation(CurrentFrameLocation + FVector(0, AvoidDirectionLength, 0));
// // 		}
// // 	}
// }



void UZSplineMovementComponent::AvoidToMulticast_Implementation(EAvoidDirection value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("multicast AvoidTo")));
	auto bike = Cast<AZBicycleCharacter>(player);
	if (bike)
	{
		auto Attractor = bike->GetMeshAttraction();
		if (Attractor)
		{
			float AvoidDirectionLength;
			switch (value)
			{
			case EAvoidDirection::Left:
			{
				AvoidDirectionLength = -AvoidLength;
				AvoidDirection = EAvoidDirection::Left;
			}
			break;
			case EAvoidDirection::Right:
			{
				AvoidDirectionLength = AvoidLength;
				AvoidDirection = EAvoidDirection::Right;
			}
			break;
			}

			FVector CurrentFrameLocation = Attractor->GetRelativeLocation();
			Attractor->SetRelativeLocation(CurrentFrameLocation + FVector(0, AvoidDirectionLength, 0));
		}
	}
}

void UZSplineMovementComponent::SetFramePositionToServer_Implementation(class AZBicycleCharacter* bike)
{
	SetFramePositionMulticast(bike);
}

void UZSplineMovementComponent::SetFramePositionMulticast_Implementation(class AZBicycleCharacter* bike)
{
	auto Frame = bike->GetFrame();
	auto MeshAttraction = bike->GetMeshAttraction();

	FVector CurrentLocation = Frame->GetRelativeLocation();
	FVector NewRelativeLocation = MeshAttraction->GetRelativeLocation();
	NewRelativeLocation.Z = CurrentLocation.Z;

    NewRelativeLocation = FMath::Lerp(CurrentLocation, NewRelativeLocation, GetWorld()->DeltaTimeSeconds);

	bike->CurrenFrameRelativeLocation = NewRelativeLocation;

	Frame->SetRelativeLocation(NewRelativeLocation, false, 0, ETeleportType::TeleportPhysics);

	auto childActor = bike->testChildActor->GetChildActor();
	if (childActor)
	{
		childActor->SetActorLocation(MeshAttraction->GetComponentLocation(), false, 0, ETeleportType::TeleportPhysics);
	}
}


void UZSplineMovementComponent::SetInitSplineComponentWithTag(FName SplineActorTag)
{

	// get SplineActor's Tag From GameInstance
	auto GameInstance = Cast<UZGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	UE_LOG(LogTemp,Warning,TEXT("SetInitSplineComponentWithTag 1: %s"), *SplineActorTag.ToString());

	// Get All Actors Of Class With Tag 
	TArray<AActor*> InitSplineActor;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AZSplineActor::StaticClass(), SplineActorTag, InitSplineActor);


	// loop for find TargetSpline
	for (auto tempSpline : InitSplineActor)
	{
		
		// 
		auto tempSplineActor = Cast<AZSplineActor>(tempSpline);
		UE_LOG(LogTemp,Warning,TEXT("Route : %s"), *tempSplineActor->Tags[0].ToString());
		if (tempSplineActor->ActorHasTag(SplineActorTag))
		{
			currentSpline = tempSplineActor->SplineComponent;

			FString TagString = SplineActorTag.ToString();
			UE_LOG(LogTemp, Warning, TEXT("Collect Route: %s"), *TagString);

			break;
		}
	}


}

void UZSplineMovementComponent::SetCurrentSplineComponent(const AActor* SplineActor)
{
	
	auto NewSplineActor = Cast<AZSplineActor>(SplineActor);
	if (NewSplineActor)
	{
		auto NewSplineComponent = NewSplineActor->SplineComponent;
		if( NewSplineComponent != nullptr )
		{
			currentSpline = NewSplineComponent;

			FString text = "ZSplineMovementComponent::Set New SplineComponent to ";
			text += currentSpline->GetName();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *text);
 
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("ZSplineMovementComponent::New Component is not a SplineComponent"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT(" ZSplineMovementComponent::New Actor is not a ZSplineActor"));
	



}








