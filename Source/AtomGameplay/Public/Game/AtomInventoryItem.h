// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomPickup.h"
#include "UObject/Object.h"
#include "AtomInventoryItem.generated.h"

class UAtomInventoryComponent;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "Atom")
class ATOMGAMEPLAY_API UAtomInventoryItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Atom|Inventory")
	UAtomInventoryComponent* OwningInventory;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Inventory")
	FAtomPickupInfo PickupInfo;

	// Override this function to implement custom logic for when the item is used.
	UFUNCTION(BlueprintNativeEvent, Category = "Atom|Inventory")
	void OnUsed(AActor* User);

	// Override this function to implement custom logic for when the item is dropped.
	UFUNCTION(BlueprintNativeEvent, Category = "Atom|Inventory")
	void OnDropped(AActor* User);

	// Override this function to implement custom logic for when the item is discarded.
	UFUNCTION(BlueprintNativeEvent, Category = "Atom|Inventory")
	void OnDiscarded(AActor* User);
};
