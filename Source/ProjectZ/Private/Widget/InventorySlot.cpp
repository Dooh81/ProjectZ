// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InventorySlot.h"

#include "Blueprint/WidgetTree.h"
#include "Components/GridPanel.h"

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UInventorySlot::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UInventorySlot::GenerateSlot(FInventoryItemData Data, EInventoryItemType Type, bool isOn, UGridPanel* GridPanel)
{
	TextSlot->SetText(FText::FromString(Data.ItemName));
	
	InventoryItemButton->WidgetStyle.Normal.SetResourceObject(Data.Image);
	InventoryItemButton->WidgetStyle.Hovered.SetResourceObject(Data.Image);
	InventoryItemButton->WidgetStyle.Pressed.SetResourceObject(Data.Image);
	InventoryItemButton->WidgetStyle.Disabled.SetResourceObject(Data.Image);
	
	InventoryItemButton->SettingItemType(Data, Type);

	if(!isOn)
	{
		//UE_LOG(LogTemp,Warning,TEXT("isOff Data %d"), Data.ItemIndex);
		Image_bg->SetColorAndOpacity(EquippedColor);
		Image_Equipped->SetVisibility(ESlateVisibility::Visible);
	}
	InventoryItemButton->SetIsEnabled(isOn);

	InventoryItemButton->SetGridPanel(GridPanel);
	
	//UE_LOG(LogTemp,Warning,TEXT("Grid Panel Count %d"), GridPanel->GetChildrenCount());
}

void UInventorySlot::SetButtonStyle(bool Enabled)
{
	if(Enabled)
	{
		Image_bg->SetColorAndOpacity(DefaultColor);
		Image_Equipped->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Image_bg->SetColorAndOpacity(EquippedColor);
		Image_Equipped->SetVisibility(ESlateVisibility::Visible);
	}
	
	InventoryItemButton->SetIsEnabled(Enabled);
}
