// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZCyclistAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZCyclistAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxWalkSpeed = 0;

};
