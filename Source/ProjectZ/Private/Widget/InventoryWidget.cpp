// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventoryWidget.h"

#include "WebSocketGameInstanceSubSystem.h"
#include "ZGameInstance.h"
#include "Blueprint/WidgetTree.h"
#include "Data/InventoryItemData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/InventorySlot.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	

	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance)
	{
		Items = ZGameInstance->GetAvatarInfo().Items;

		UE_LOG(LogTemp,Warning,TEXT("Items Count: %d"), Items.Num());
		// UWebSocketGameInstanceSubSystem* WebSocketGameInstanceSubSystem = ZGameInstance->GetSubsystem<UWebSocketGameInstanceSubSystem>();
		// WebSocketGameInstanceSubSystem->OnSentMessage(E_WS_Command::ChangePlayerEquipment, "??");
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////
	
	//classification
	for (int i = 0; i < Items.Num(); i++)
	{
		TArray<int32> LoopNum;

		int32 DummyInt;
		DummyInt = Items[i];
		while (DummyInt > 0)
		{
			LoopNum.Add(DummyInt % 10);
			DummyInt /= 10;
		}

		switch (LoopNum[LoopNum.Num() - 1])
		{
		case 1:
			FrameItems.Add(Items[i]);
			break;
		case 2:
			WheelItems.Add(Items[i]);
			break;
		case 3:
			HeadgearItems.Add(Items[i]);
			break;
		case 4:
			GlassesItems.Add(Items[i]);
			break;
		case 5:
			JerseyItems.Add(Items[i]);
			break;
		case 6:
			GlovesItems.Add(Items[i]);
			break;
		case 7:
			SocksItems.Add(Items[i]);
			break;
		case 8:
			ShoesItems.Add(Items[i]);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("other data? %d"), Items[i]);
			break;
		}
	}

	//sorting asc
	FrameItems.Sort();
	WheelItems.Sort();
	HeadgearItems.Sort();
	GlassesItems.Sort();
	JerseyItems.Sort();
	GlovesItems.Sort();
	SocksItems.Sort();
	ShoesItems.Sort();

	//OnClickedInventory 버튼이벤트
	FScriptDelegate Button_MainMenuDelegate;
	Button_MainMenuDelegate.BindUFunction(this, "OnClickedMainMenu");
	Button_Main->OnClicked.Add(Button_MainMenuDelegate);

	TArray<AActor*> PartModels;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APartsModel::StaticClass(),PartModels);
	if(PartModels.Num() !=0)
	{
		PartsModel = Cast<APartsModel>(PartModels[0]);
	}
}

void UInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	//Initialization
	Items.Empty();
	FrameItems.Empty();
	WheelItems.Empty();
	HeadgearItems.Empty();
	GlassesItems.Empty();
	JerseyItems.Empty();
	GlovesItems.Empty();
	SocksItems.Empty();
	ShoesItems.Empty();
}

void UInventoryWidget::CallPartsItems(EInventoryItemType ItemType)
{
	GridPanel->ClearChildren();
	
	int32 SelectEquippedIndex = 00000000;

	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if (ZGameInstance)
	{
		FPlayerEquipmentInfo PlayerEquipmentInfo = ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo;


		switch (ItemType)
		{
		default:
			break;
		case EInventoryItemType::EFrame:
			SelectItems = FrameItems;
			SelectEquippedIndex = PlayerEquipmentInfo.FrameIndex;
			break;
		case EInventoryItemType::EWheel:
			SelectItems = WheelItems;
			SelectEquippedIndex = PlayerEquipmentInfo.WheelIndex;
			break;
		case EInventoryItemType::EHeadgear:
			SelectItems = HeadgearItems;
			SelectEquippedIndex = PlayerEquipmentInfo.HeadgearIndex;
			break;
		case EInventoryItemType::EGlasses:
			SelectItems = GlassesItems;
			SelectEquippedIndex = PlayerEquipmentInfo.GlassesIndex;
			break;
		case EInventoryItemType::EJersey:
			SelectItems = JerseyItems;
			SelectEquippedIndex = PlayerEquipmentInfo.JerseyIndex;
			break;
		case EInventoryItemType::EGloves:
			SelectItems = GlovesItems;
			SelectEquippedIndex = PlayerEquipmentInfo.GlovesIndex;
			break;
		case EInventoryItemType::ESocks:
			SelectItems = SocksItems;
			SelectEquippedIndex = PlayerEquipmentInfo.SocksIndex;
			break;
		case EInventoryItemType::EShoes:
			SelectItems = ShoesItems;
			SelectEquippedIndex = PlayerEquipmentInfo.ShoesIndex;
			break;
		}
	}

	int32 MaxColSize = 3;
	int32 Row=0;
	int32 Col=0;

	for (int i = 0; i < SelectItems.Num(); i++)
	{
		//UE_LOG(LogTemp,Warning,TEXT("HeadgearItems[%d] : value - %d"),i,HeadgearItems[i]);
		int32 compareNum = SelectItems[i];

		FInventoryItemData* data = InventoryTable->FindRow<FInventoryItemData>(
			*FString::FromInt(compareNum), TEXT(""));

		if (data != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Get data name : %s"), *data->ItemName);
			UE_LOG(LogTemp, Log, TEXT("SelectEquippedIndex : %d"), SelectEquippedIndex);
			UE_LOG(LogTemp,Error,TEXT("SelectNum = %d"),data->ItemIndex);
			
			UUserWidget* SlotWidget = CreateWidget(GetWorld(), InventorySlotClass);

			bool isOn = true;
			if (data->ItemIndex == SelectEquippedIndex)
			{
				isOn = false;
				UE_LOG(LogTemp,Error,TEXT("SelectNum = %d"),data->ItemIndex);
			}

			Col = i;
			if(Col >= MaxColSize) Col= Col % MaxColSize;
			Row = i / MaxColSize;
			GridPanel->AddChildToGrid(SlotWidget, Row, Col);
			Cast<UInventorySlot>(SlotWidget)->GenerateSlot(*data, ItemType, isOn, GridPanel);
			//UE_LOG(LogTemp,Warning,TEXT("Row : %d, Col : %d"), Row, Col);
		}
	}
	
}

void UInventoryWidget::AllDisablePartsModel()
{
	PartsModel->OnAllDisabledHighlight();
}

void UInventoryWidget::ShowHighlight(EInventoryItemType ItemType)
{
	PartsModel->SetEnableHighlight(ItemType);
}

void UInventoryWidget::OnClickedMainMenu()
{

	// UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	// if(ZGameInstance)
	// {
	// 	Items = ZGameInstance->GetAvatarInfo().Items;
	//
	// 	UE_LOG(LogTemp,Warning,TEXT("Items Count: %d"), Items.Num());
		
	// }
	
	UZGameInstance* ZGameInstance = Cast<UZGameInstance>(GetGameInstance());
	if(ZGameInstance!= nullptr)
	{
		TArray<int32> EquipmentItem;
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.FrameIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.WheelIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.HeadgearIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.GlassesIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.JerseyIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.GlovesIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.SocksIndex);
		EquipmentItem.Add(ZGameInstance->GetAvatarInfo().PlayerEquipmentInfo.ShoesIndex);

		for (int i = 0 ; i< EquipmentItem.Num(); i++)
		{
			UE_LOG(LogTemp,Warning,TEXT("Item Index: %d"), EquipmentItem[i]);
		}
		
		UWebSocketGameInstanceSubSystem* WebSocketGameInstanceSubSystem = ZGameInstance->GetSubsystem<UWebSocketGameInstanceSubSystem>();
		WebSocketGameInstanceSubSystem->OnSentMessage(E_WS_Command::ChangePlayerEquipment, EquipmentItem);
		ZGameInstance->LoadingAfterLoadLevel(ELevelName::EMainMenu);
	}
}
