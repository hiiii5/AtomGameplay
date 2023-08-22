// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtomInventoryComponent.generated.h"

struct FAtomPickupInfo;
// This is like saying we should include the header this belongs to
// However, to reduce inclusions (preventing a circular dependency)
class UAtomInventoryItem;

UCLASS(meta=(BlueprintSpawnableComponent), ClassGroup="Atom")
class ATOMGAMEPLAY_API UAtomInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomInventoryComponent();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Atom|Inventory")
	TArray<UAtomInventoryItem*> Items;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Atom|Inventory")
	void CreateInventoryItem(FAtomPickupInfo PickupInfo, UAtomInventoryItem*& Item);
	
	UFUNCTION(BlueprintCallable, Category="Atom|Inventory")
	void AddToInventory(UAtomInventoryItem* ItemToAdd);

	UFUNCTION(BlueprintCallable, Category="Atom|Inventory")
	void GetTotalQuantity(FString ItemName, int32& Quantity);
	
	UFUNCTION(BlueprintCallable, Category="Atom|Inventory")
	bool RemoveFromInventory(FString ItemName, int32 QuantityToRemove);
	
	UFUNCTION(BlueprintCallable, Category="Atom|Inventory")
	void FindItemInInventory(FString ItemName, UAtomInventoryItem*& Item);
};
