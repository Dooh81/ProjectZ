// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commons.generated.h"
/*
 *Enum
 */

UENUM(BlueprintType)
enum class ELevelName : uint8
{
	EMainMenu UMETA(DisplayName = "MainMenu"),
	ECityPark UMETA(DisplayName = "CityPark"),
	ESF UMETA(DisplayName = "Sanfransico"),
	ETrack_Cycling_Cinematic UMETA(DisplayName = "Inventory"),
	ETest UMETA(DisplayName = "Test"),
};

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
	EFrame UMETA(DisplayName = "Frame"),
	EWheel UMETA(DisplayName = "Wheel"),
	EUser UMETA(DisplayName = "User"),
	EHeadgear UMETA(DisplayName = "Headgear"),
	EGlasses UMETA(DisplayName = "Glasses"),
	EJersey UMETA(DisplayName = "Jersey"),
	EGloves UMETA(DisplayName = "Gloves"),
	ESocks UMETA(DisplayName = "Socks"),
	EShoes UMETA(DisplayName = "Shoes"),
};

UENUM(BlueprintType)
enum class ECountry : uint8
{
	EUN UMETA(DisplayName = "UN"),
	EBRA UMETA(DisplayName = "BRA"),
	ECHN UMETA(DisplayName = "CHN"),
	EDEU UMETA(DisplayName = "DEU"),
	EESP UMETA(DisplayName = "ESP"),
	EFRA UMETA(DisplayName = "FRA"),
	EJPN UMETA(DisplayName = "JPN"),
	EKOR UMETA(DisplayName = "KOR"),
	EPRK UMETA(DisplayName = "PRK"),
	ERUS UMETA(DisplayName = "RUS"),
	EUSA UMETA(DisplayName = "USA"),
};

UENUM(BlueprintType)
enum class EGenderType : uint8
{
	EMale UMETA(DisplayName = "Male"),
	EFemale UMETA(DisplayName = "Female")
};

UENUM(BlueprintType)
enum class EBodyColor : uint8
{
	EYellow UMETA(DisplayName = "Yellow"),
	EWhite UMETA(DisplayName = "White"),
	EBlack UMETA(DisplayName = "Black"),
};

UENUM(BlueprintType)
enum class EBicycleType : uint8
{
	ERoad UMETA(DisplayName = "Road"),
	EMTB UMETA(DisplayName = "MTB"),
};


/*
 *STRUCT
 */


USTRUCT(Atomic, BlueprintType)
struct FPlayerEquipmentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 FrameIndex; //1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 WheelIndex; //2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 HeadgearIndex; //3

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 GlassesIndex; //4

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 JerseyIndex; //5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 GlovesIndex; //6

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 SocksIndex; //7

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerEquipmentInfo")
	int32 ShoesIndex; //8
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerAvatarInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	FString UserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	ECountry Country;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	int64 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	int64 Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	EGenderType Gender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	EBodyColor BodyColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	EBicycleType Bicycle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	FPlayerEquipmentInfo PlayerEquipmentInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerAvatarInfo")
	TArray<int32> Items;
};

USTRUCT(Atomic, BlueprintType)
struct FDisplayUserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DisplayPlayer")
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DisplayPlayer")
	ECountry Country;
};

USTRUCT(Atomic, BlueprintType)
struct FBicycleCalcData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BicycleCalcData")
	FString TotalKm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BicycleCalcData")
	FString PlayerSpeed;
};

USTRUCT(Atomic, BlueprintType)
struct FSendCommandValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SendCommandValue")
	FString Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SendCommandValue")
	TArray<int32> Nums;
	//required develop AchievementsInfo 
};
/**
 * 
 */
class PROJECTZ_API Commons
{
public:
	Commons();
	~Commons();
};

template<typename TEnum>
static FORCEINLINE TEnum ConvertIntToEnum(const int32 Value)
{
	return static_cast<TEnum>(Value);
}

template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue)).Mid(1);
}

template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToFullString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue));
}
