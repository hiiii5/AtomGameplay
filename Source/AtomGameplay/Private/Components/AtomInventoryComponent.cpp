// @AngelGuard


#include "Components/AtomInventoryComponent.h"

#include "Data/AtomPickupData.h"
#include "Game/AtomPickup.h"


// Sets default values for this component's properties
UAtomInventoryComponent::UAtomInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAtomInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAtomInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAtomInventoryComponent::AddPickup(AAtomPickup* Pickup)
{
	ensure(Pickup);

	if (Inventory.Num() >= MaxInventorySlots)
	{
		return false;
	}
	
	FAtomPickupInfo PickupInfo = Pickup->PickupInfo;

	if (!Contains(PickupInfo.PickupData->Name.ToString()))
	{
		Inventory.Add(FAtomInventoryInfo{PickupInfo.PickupData, PickupInfo.Quantity});
		return true;
	}
	
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].PickupData == PickupInfo.PickupData)
		{
			Inventory[i].Quantity += PickupInfo.Quantity;
			return true;
		}
	}

	return false;
}

bool UAtomInventoryComponent::Contains(const FString& Name)
{
	for (const FAtomInventoryInfo Element : Inventory)
	{
		if (Element.PickupData->Name.ToString() == Name)
		{
			return true;
		}
	}

	return false;
}

bool UAtomInventoryComponent::ContainsPickup(UAtomPickupData* PickupData)
{
	if (!PickupData)
	{
		return false;
	}

	return Contains(PickupData->Name.ToString());
}

int32 UAtomInventoryComponent::IndexOf(const FString& Name)
{
	int32 FoundIndex = -1;
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].PickupData->Name.ToString() == Name)
		{
			FoundIndex = i;
			break;
		}
	}

	return FoundIndex;
}

int32 UAtomInventoryComponent::IndexOfPickup(UAtomPickupData* PickupData)
{
	if (!PickupData)
	{
		return -1;
	}
	return IndexOf(PickupData->Name.ToString());
}

int32 UAtomInventoryComponent::QuantityOf(const FString& Name)
{
	int32 Quantity = 0;
	for (const FAtomInventoryInfo Element : Inventory)
	{
		if (Element.PickupData->Name.ToString() == Name)
		{
			Quantity = Element.Quantity;
			break;
		}
	}

	return Quantity;
}

int32 UAtomInventoryComponent::QuantityOfPickup(UAtomPickupData* PickupData)
{
	if (!PickupData)
	{
		return 0;
	}

	return QuantityOf(PickupData->Name.ToString());
}

void UAtomInventoryComponent::RemoveAt(const int32 Index)
{
	if (Index < 0 || Index >= Inventory.Num())
	{
		return;
	}

	Inventory.RemoveAt(Index);
}

void UAtomInventoryComponent::Remove(const FString& Name)
{
	const int32 Index = IndexOf(Name);
	if (Index < 0)
	{
		return;
	}

	RemoveAt(Index);
}

void UAtomInventoryComponent::RemovePickup(UAtomPickupData* PickupData)
{
	const int32 Index = IndexOfPickup(PickupData);
	if (Index < 0)
	{
		return;
	}

	RemoveAt(Index);
}

void UAtomInventoryComponent::RemoveCount(const FString& Name, const int32 Count)
{
	const int32 TotalQuantity = QuantityOf(Name);
	if (TotalQuantity <= 0)
	{
		return;
	}

	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].PickupData->Name.ToString() == Name)
		{
			// If the quantity of this item is less than the count to remove, then get the remainder to remove and continue finding more of the elements
			if (Inventory[i].Quantity < Count)
			{
				RemoveAt(i);
				RemoveCount(Name, Count - Inventory[i].Quantity);
				return;
			}

			// Otherwise, remove the count from the quantity and return
			Inventory[i].Quantity -= Count;
			if (Inventory[i].Quantity <= 0)
			{
				RemoveAt(i);
			}
			return;
		}
	}
}

void UAtomInventoryComponent::RemovePickupCount(UAtomPickupData* PickupData, const int32 Count)
{
	if (!PickupData)
	{
		return;
	}

	RemoveCount(PickupData->Name.ToString(), Count);
}

void UAtomInventoryComponent::Clear()
{
	Inventory.Empty();
}

int32 UAtomInventoryComponent::Count()
{
	return Inventory.Num();
}

int32 UAtomInventoryComponent::RemainingSlotCount()
{
	return MaxInventorySlots - Inventory.Num();
}

