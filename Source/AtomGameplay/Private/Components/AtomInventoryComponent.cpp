// @AngelGuard


#include "Components/AtomInventoryComponent.h"

#include "Data/AtomPickupData.h"
#include "Game/AtomInventoryItem.h"

// Sets default values for this component's properties
UAtomInventoryComponent::UAtomInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize our array of inventory items
	Items = TArray<UAtomInventoryItem*>();
}

void UAtomInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAtomInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAtomInventoryComponent::CreateInventoryItem(const FAtomPickupInfo PickupInfo, UAtomInventoryItem*& Item)
{
	// Create a new inventory item
	Item = NewObject<UAtomInventoryItem>();
	Item->PickupInfo = PickupInfo;
}

void UAtomInventoryComponent::AddToInventory(UAtomInventoryItem* ItemToAdd)
{
	FAtomPickupInfo PickupInfo = ItemToAdd->PickupInfo;
	float Quantity = PickupInfo.Quantity;
	bool bIsStackable = PickupInfo.bIsStackable;

	if (!bIsStackable)
	{
		Items.Add(ItemToAdd);

		return;
	}

	// The item to add might have multiple entries in the items list, and they might not be stacked
	// We need to check if we have any of the item in our inventory, and if so, add to the quantity
	for (int i = 0; i < Items.Num(); i++)
	{
		UAtomInventoryItem* CurrentItem = Items[i];
		FAtomPickupInfo CurrentItemPickupInfo = CurrentItem->PickupInfo; //a copy
		float ItemQuantity = CurrentItemPickupInfo.Quantity;
		bool bItemIsStackable = CurrentItemPickupInfo.bIsStackable;

		// If the item is stackable, and the item we're adding is stackable, and the item names match
		bool bDoesNameMatch = CurrentItemPickupInfo.PickupData->Name.ToString() == PickupInfo.PickupData->Name.ToString();
		bool bCanStack = bItemIsStackable && bIsStackable;
		if (bDoesNameMatch && bCanStack)
		{
			// Add the quantity of the item we're adding to the quantity of the item in the inventory
			// Check if the quantity is greater than the stack size, and if so, add the remainder to the next item
			float NewQuantity = ItemQuantity + Quantity;
			float MaxStackQuantity = CurrentItemPickupInfo.MaxStackQuantity;
			if (NewQuantity > MaxStackQuantity)
			{
				// Set the quantity of the item in the inventory to the max stack quantity
				CurrentItemPickupInfo.Quantity = MaxStackQuantity;
				CurrentItem->PickupInfo = CurrentItemPickupInfo; //set equal to the copy

				// Set the quantity of the item we're adding to the remainder
				// This assumes that the quantity we are adding can not be greater than the max stack quantity
				PickupInfo.Quantity = NewQuantity - MaxStackQuantity;
				ItemToAdd->PickupInfo = PickupInfo;

				// Add the item to add to the inventory if the remaining quantity is > 0
				if (PickupInfo.Quantity > 0)
				{
					Items.Add(ItemToAdd);
				}
				return;
			}

			// Set the quantity of the item in the inventory to the new quantity
			CurrentItemPickupInfo.Quantity = NewQuantity;
			CurrentItem->PickupInfo = CurrentItemPickupInfo;

			return;
		}
	}
}

void UAtomInventoryComponent::GetTotalQuantity(FString ItemName, int32& Quantity)
{
	Quantity = 0;

	for (UAtomInventoryItem* Item : Items)
	{
		FAtomPickupInfo ItemPickupInfo = Item->PickupInfo;
		FString ItemPickupName = ItemPickupInfo.PickupData->Name.ToString();
		if (ItemPickupName == ItemName)
		{
			Quantity += ItemPickupInfo.Quantity;
		}
	}
}

bool UAtomInventoryComponent::RemoveFromInventory(FString ItemName, int32 QuantityToRemove)
{
	UAtomInventoryItem* ItemToRemove = nullptr;
	FindItemInInventory(ItemName, ItemToRemove);

	if (!ItemToRemove)
	{
		return false;
	}

	int32 TotalQuantity;
	GetTotalQuantity(ItemName, TotalQuantity);

	if (TotalQuantity < QuantityToRemove)
	{
		return false;
	}

	for (int i = 0; i < Items.Num(); i++)
	{
		UAtomInventoryItem* CurrentItem = Items[i];
		FAtomPickupInfo CurrentItemPickupInfo = CurrentItem->PickupInfo;
		FString CurrentItemName = CurrentItemPickupInfo.PickupData->Name.ToString();

		// If the item we are looking at does not match the item we want to remove, continue to the next item
		if (ItemName != CurrentItemName)
		{
			continue;
		}

		int32 OriginalQuantityToRemove = QuantityToRemove;
		// Subtract the amount that is in the current slot from the amount we want to remove
		QuantityToRemove -= CurrentItemPickupInfo.Quantity;
		// Get the difference of the original amount we wanted to remove and the amount that we have left to remove
		int32 Difference = OriginalQuantityToRemove - QuantityToRemove;
		// Subtract the difference from the current items quantity
		CurrentItemPickupInfo.Quantity -= Difference;

		if (CurrentItemPickupInfo.Quantity <= 0)
		{
			Items.RemoveAt(i);
			i--;
		}

		if (QuantityToRemove <= 0)
		{
			return true;
		}
	}

	return false;
}

void UAtomInventoryComponent::FindItemInInventory(FString ItemName, UAtomInventoryItem*& Item)
{
	Item = nullptr;

	// Loop over every item in the inventory, until we find an who's pickup has a matching name
	for (int i = 0; i < Items.Num(); i++)
	{
		UAtomInventoryItem* CurrentItem = Items[i];
		FAtomPickupInfo CurrentItemPickupInfo = CurrentItem->PickupInfo;
		FString CurrentItemName = CurrentItemPickupInfo.PickupData->Name.ToString();

		if (ItemName == CurrentItemName)
		{
			Item = CurrentItem;
			return;
		}
	}
}
