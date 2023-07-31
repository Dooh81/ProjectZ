// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "ZSplineMovementComponent.generated.h"



// splinemovement state
UENUM(BlueprintType)
enum class ESplineMoveState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	FindRoute 	UMETA(DisplayName = "FindRoute"),	// Search a track 
	OnRoute UMETA(DisplayName = "OnRoute"),			// Set  Movement Direction and Input 
};

// avoidance position direction
UENUM(BlueprintType)
enum class EAvoidDirection : uint8
{
	Left UMETA(DisplayName = "Left"),
	Center UMETA(DisplayName = "Center"),
	Right UMETA(DisplayName = "Right"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTZ_API UZSplineMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UZSplineMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MoveState")
		ESplineMoveState moveState;	// movement state variable


	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Avoidance")
		EAvoidDirection AvoidDirection;


	UPROPERTY()
		class USplineComponent* currentSpline;	// which the spline that player have to follow



private:

	void IdleProcess();			// tick -> switch-case -> idle state process function

	void FindRouteProcess();	// tick -> switch-case -> FindRoute state process function

	void OnRouteProcess();		// tick -> switch-case -> OnRoute state process function

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> splineActors;

private:
	class AProjectZCharacter* player;	// owner character 

	//TArray<AActor*> splineActors;		// all splines on the level


	class UZBikeAnimInstance* BikeAnim;
	class UZCyclistAnimInstance* CyclistAnim;

public:
	UPROPERTY()
		bool DirectionFlag;		// temporary test variable for movement direction


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "movement");
	float avoidMovementRadius;	// local avoid movement logic Radius
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "movement");
	float movementTargetDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "movement");
	float moveWeight;

	UPROPERTY(EditAnywhere, Replicated)
	FVector_NetQuantize moveDirection;
	UPROPERTY(EditAnywhere, Replicated)
	float closestKey;			
	UPROPERTY(EditAnywhere, Replicated)
	FVector_NetQuantize targetLocation;	


private:
	UPROPERTY()
		int32  AvoidanceUID;		// GetWorld()-> AvoidanceManager -> UID


	UPROPERTY()
		bool bAvoidance = false;


public:
	UFUNCTION()
	void SetAvoidanceUID(int32 value)	{ AvoidanceUID = value; } // not use recently


	UFUNCTION()
	void SetAvoidDirection(EAvoidDirection value)	{ AvoidDirection = value; };

	//
	UFUNCTION(Server, Reliable)
	void AvoidToServer(EAvoidDirection value);
	void AvoidToServer_Implementation(EAvoidDirection value);

	UFUNCTION(NetMulticast, Reliable)
	void AvoidToMulticast(EAvoidDirection value);
	void AvoidToMulticast_Implementation(EAvoidDirection value);
	//


	//
	UFUNCTION(Server, Reliable)
	void SetFramePositionToServer(class AZBicycleCharacter* bike);
	void SetFramePositionToServer_Implementation(class AZBicycleCharacter* bike);


	UFUNCTION(NetMulticast, Reliable)
	void SetFramePositionMulticast(class AZBicycleCharacter* bike);
	void SetFramePositionMulticast_Implementation(class AZBicycleCharacter* bike);
	//

	
	
	UFUNCTION()
	void SetInitSplineComponentWithTag(FName SplineActorTag);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSplineComponent(const class AActor* SplineActor);

	


private:
	UFUNCTION()
		void FollowSplineProcess();

	UFUNCTION(Server, Reliable)
		void ReturnToCenterServer(class AZBicycleCharacter* bike);
		void ReturnToCenterServer_Implementation(class AZBicycleCharacter* bike);

	UFUNCTION(NetMulticast, Reliable)
		void ReturnToCenterMulticast(class AZBicycleCharacter* bike);
		void ReturnToCenterMulticast_Implementation(class AZBicycleCharacter* bike);

	UFUNCTION()
		void AvoidanceProcess(class AZBicycleCharacter* bike);

public:
	UPROPERTY(BlueprintReadOnly, Category = "Anim")
		FRotator animWheelRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim");
		float rearWheelDistance;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool bDebug;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avoidance")
		float AvoidLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avoidance")
		float LineTraceLength;

	

private:

};
