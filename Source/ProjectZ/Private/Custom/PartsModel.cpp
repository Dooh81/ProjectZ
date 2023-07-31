// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom/PartsModel.h"

#include "WebSocketGameInstanceSubSystem.h"
#include "ZGameInstance.h"

// Sets default values
APartsModel::APartsModel()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(DefaultRoot);

	Frame = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FrameDummy"));
	Frame->SetupAttachment(DefaultRoot);

	Wheel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WheelDummy"));
	Wheel->SetupAttachment(DefaultRoot);

	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceDummy"));
	Face->SetupAttachment(DefaultRoot);

	Headgear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadgearDummy"));
	Headgear->SetupAttachment(DefaultRoot);

	Glasses = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlassesDummy"));
	Glasses->SetupAttachment(DefaultRoot);

	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesDummy"));
	Gloves->SetupAttachment(DefaultRoot);

	Upper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperDummy"));
	Upper->SetupAttachment(DefaultRoot);

	Lower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerDummy"));
	Lower->SetupAttachment(DefaultRoot);

	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesDummy"));
	Shoes->SetupAttachment(DefaultRoot);
}

// Called when the game starts or when spawned
void APartsModel::BeginPlay()
{
	Super::BeginPlay();

	InitAllParts();
}


// Called to bind functionality to input
void APartsModel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APartsModel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FrameMaterialInstance = Frame->CreateDynamicMaterialInstance(0);
	FrameBackwardMaterialInstance = Frame->CreateDynamicMaterialInstance(1);
	WheelMaterialInstance = Wheel->CreateDynamicMaterialInstance(0);
	FaceMaterialInstance = Face->CreateDynamicMaterialInstance(0);
	GlovesMaterialInstance = Gloves->CreateDynamicMaterialInstance(0);
	HeadgearMaterialInstance = Headgear->CreateDynamicMaterialInstance(0);
	GlassesMaterialInstance = Glasses->CreateDynamicMaterialInstance(1);
	UpperMaterialInstance = Upper->CreateDynamicMaterialInstance(0);
	LowerMaterialInstance = Lower->CreateDynamicMaterialInstance(0);
	ShoesMaterialInstance = Shoes->CreateDynamicMaterialInstance(0);
}

void APartsModel::ChangeParts(FInventoryItemData Data, EInventoryItemType Type)
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance)
	{
		FPlayerEquipmentInfo PlayerEquipmentInfo = ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo;


		//UE_LOG(LogTemp, Warning, TEXT("ChangeParts Data: %s"), *Data.ItemName);
		int32 DummyInt = Data.ItemIndex;
		DummyInt %= 10;

		switch (Type)
		{
		default:
			break;
		case EInventoryItemType::EFrame:
			PlayerEquipmentInfo.FrameIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);

			switch (DummyInt)
			{
			default:
				break;
			case 0:
				FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardG);
				FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardG);
				break;
			case 1:
				FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardB);
				FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardB);
				break;
			case 2:
				FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardR);
				FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardR);
				break;
			}
			break;
		case EInventoryItemType::EWheel:
			PlayerEquipmentInfo.WheelIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);
			switch (DummyInt)
			{
			default:
				break;
			case 0:
				WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelBK);
				break;
			case 1:
				WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelG);
				break;
			case 2:
				WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelB);
				break;
			case 3:
				WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelR);
				break;
			}

			break;
		case EInventoryItemType::EHeadgear:
			PlayerEquipmentInfo.HeadgearIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);

		//UE_LOG(LogTemp, Warning, TEXT("last num %d"), DummyInt);
			switch (DummyInt)
			{
			default:
				break;
			case 0:
				HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
				break;
			case 1:
				HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
				break;
			case 2:
				HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
				break;
			}
			break;
		case EInventoryItemType::EGlasses:
			PlayerEquipmentInfo.GlassesIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);

			switch (DummyInt)
			{
			default:
				break;
			case 0:
				GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesGreen);
				break;
			case 1:
				GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesBlue);
				break;
			case 2:
				GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesRed);
				break;
			}
			break;
		case EInventoryItemType::EJersey:
			PlayerEquipmentInfo.JerseyIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);

			switch (DummyInt)
			{
			default:
				break;
			case 0:
				UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
				LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
				break;
			case 1:
				UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
				LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
				break;
			case 2:
				UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
				LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
				break;
			}
			break;
		case EInventoryItemType::EGloves:
			PlayerEquipmentInfo.GlovesIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);

			switch (DummyInt)
			{
			default:
				break;
			case 0:
				GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
				break;
			case 1:
				GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
				break;
			case 2:
				GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
				break;
			}
			break;
		case EInventoryItemType::ESocks:
			PlayerEquipmentInfo.SocksIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);
			break;
		case EInventoryItemType::EShoes:
			PlayerEquipmentInfo.ShoesIndex = Data.ItemIndex;
			ZGameInstance->SetEquipmentInfo(PlayerEquipmentInfo);
			switch (DummyInt)
			{
			default:
				break;
			case 0:
				ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
				break;
			case 1:
				ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWhite);
				break;
			}
			break;
		}
	}
}

void APartsModel::InitAllParts()
{
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance)
	{
		FPlayerEquipmentInfo PlayerEquipmentInfo = ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo;
		int32 FrameIndex = PlayerEquipmentInfo.FrameIndex;
		int32 WheelIndex = PlayerEquipmentInfo.WheelIndex;
		int32 HeadgearIndex = PlayerEquipmentInfo.HeadgearIndex;
		int32 GlassesIndex = PlayerEquipmentInfo.GlassesIndex;
		int32 JerseyIndex = PlayerEquipmentInfo.JerseyIndex;
		int32 GlovesIndex = PlayerEquipmentInfo.GlovesIndex;
		int32 SocksIndex = PlayerEquipmentInfo.SocksIndex;
		int32 ShoesIndex = PlayerEquipmentInfo.ShoesIndex;

		UE_LOG(LogTemp, Warning, TEXT("Player Info FrameIndex %d"), FrameIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info WheelIndex %d"), WheelIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info Headgear %d"), HeadgearIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info GlassesIndex %d"), GlassesIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info JerseyIndex %d"), JerseyIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info GlovesIndex %d"), GlovesIndex);
		UE_LOG(LogTemp, Warning, TEXT("Player Info ShoesIndex %d"), ShoesIndex);

		FrameIndex %= 10;
		WheelIndex %= 10;
		HeadgearIndex %= 10;
		GlassesIndex %= 10;
		JerseyIndex %= 10;
		GlovesIndex %= 10;
		SocksIndex %= 10;
		ShoesIndex %= 10;


		switch (FrameIndex)
		{
		default:
			break;
		case 0:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardG);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardG);
			break;
		case 1:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardB);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardB);
			break;
		case 2:
			FrameMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameForwardR);
			FrameBackwardMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureFrameBackwardR);
			break;
		}

		switch (WheelIndex)
		{
		default:
			break;
		case 0:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelBK);
			break;
		case 1:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelG);
			break;
		case 2:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelB);
			break;
		case 3:
			WheelMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWheelR);
			break;
		}

		switch (HeadgearIndex)
		{
		default:
			break;
		case 0:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			HeadgearMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (GlassesIndex)
		{
		default:
			break;
		case 0:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesGreen);
			break;
		case 1:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesBlue);
			break;
		case 2:
			GlassesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGlassesRed);
			break;
		}

		switch (JerseyIndex)
		{
		default:
			break;
		case 0:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			UpperMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			LowerMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (GlovesIndex)
		{
		default:
			break;
		case 0:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureBlue);
			break;
		case 2:
			GlovesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureRed);
			break;
		}

		switch (ShoesIndex)
		{
		default:
			break;
		case 0:
			ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureGreen);
			break;
		case 1:
			ShoesMaterialInstance->SetTextureParameterValue(FName(TEXT("Color")), TextureWhite);
			break;
		}
	}
}

void APartsModel::OnAllDisabledHighlight()
{
	Frame->SetRenderCustomDepth(false);
	Wheel->SetRenderCustomDepth(false);
	Face->SetRenderCustomDepth(false);
	Gloves->SetRenderCustomDepth(false);
	Headgear->SetRenderCustomDepth(false);
	Glasses->SetRenderCustomDepth(false);
	Upper->SetRenderCustomDepth(false);
	Lower->SetRenderCustomDepth(false);
	Shoes->SetRenderCustomDepth(false);
}

void APartsModel::SetEnableHighlight(EInventoryItemType Type)
{
	switch (Type)
	{
	default:
		break;
	case EInventoryItemType::EFrame:
		Frame->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EWheel:
		Wheel->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EGlasses:
		Glasses->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EHeadgear:
		Headgear->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EGloves:
		Gloves->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EJersey:
		Upper->SetRenderCustomDepth(true);
		Lower->SetRenderCustomDepth(true);
		break;
	case EInventoryItemType::EShoes:
		Shoes->SetRenderCustomDepth(true);
		break;
	}
}
