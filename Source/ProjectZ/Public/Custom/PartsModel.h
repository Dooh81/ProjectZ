// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons/Commons.h"
#include "Data/InventoryItemData.h"
#include "GameFramework/Pawn.h"
#include "Containers/CircularQueue.h"
#include "PartsModel.generated.h"

UCLASS()
class PROJECTZ_API APartsModel : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APartsModel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Wheel;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Face;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Gloves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Headgear;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Glasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Upper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Lower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Parts, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Shoes;

	UPROPERTY()
	class UMaterialInstanceDynamic* FrameMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* FrameBackwardMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* WheelMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* FaceMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* GlovesMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* HeadgearMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* GlassesMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* UpperMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* LowerMaterialInstance;
	UPROPERTY()
	class UMaterialInstanceDynamic* ShoesMaterialInstance;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	
	void ChangeParts(FInventoryItemData Data, EInventoryItemType Type);

	void InitAllParts();

	void OnAllDisabledHighlight();

	void SetEnableHighlight(EInventoryItemType Type);

	
	//materials

	//texture
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGreen;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureBlue;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureRed;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWhite;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesGreen;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesBlue;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureGlassesRed;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameForwardB;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureFrameBackwardB;

	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelBK;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelR;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelG;
	UPROPERTY(EditAnywhere, Category="PartsModel")
	UTexture* TextureWheelB;
};
