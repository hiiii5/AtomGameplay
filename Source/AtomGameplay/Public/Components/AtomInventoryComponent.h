// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/AtomSaveInterface.h"
#include "AtomInventoryComponent.generated.h"

class AAtomPickup;
class UAtomPickupData;

USTRUCT(BlueprintType, Blueprintable)
struct FAtomInventoryInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Inventory")
	UAtomPickupData* PickupData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Inventory")
	int32 Quantity = 1;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomInventoryComponent : public UActorComponent, public IAtomSaveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomInventoryComponent();

	UPROPERTY(SaveGame, BlueprintReadOnly, EditAnywhere, Category = "Atom|Inventory")
	TArray<FAtomInventoryInfo> Inventory;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Inventory")
	int32 MaxInventorySlots = 10;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	bool AddPickup(AAtomPickup* Pickup);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	bool Contains(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	bool ContainsPickup(UAtomPickupData* PickupData);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 IndexOf(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 IndexOfPickup(UAtomPickupData* PickupData);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 QuantityOf(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 QuantityOfPickup(UAtomPickupData* PickupData);
	
	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void RemoveAt(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void Remove(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void RemovePickup(UAtomPickupData* PickupData);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void RemoveCount(const FString& Name, const int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void RemovePickupCount(UAtomPickupData* PickupData, const int32 Count);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 Count();
	
	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory")
	int32 RemainingSlotCount();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Atom|Inventory")
	TArray<FAtomInventoryInfo> GetInventory() const { return Inventory; }
};
