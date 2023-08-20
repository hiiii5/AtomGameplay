// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomInteractable.h"
#include "AtomPickup.generated.h"

class UAtomPickupData;

USTRUCT(BlueprintType, Blueprintable)
struct FPickupInfo
{
	GENERATED_BODY()

	FPickupInfo()
		: PickupData(nullptr),
		  Quantity(1),
		  bIsStackable(false),
		  bIsQuestItem(false),
		  bCanBeDropped(true),
		  bCanBeUsed(false)
	{
	}
	
	FPickupInfo(const int32 Quantity, const bool bIsStackable, const bool bIsQuestItem, const bool bCanBeDropped, const bool bCanBeUsed)
		: PickupData(nullptr),
		  Quantity(Quantity),
		  bIsStackable(bIsStackable),
		  bIsQuestItem(bIsQuestItem),
		  bCanBeDropped(bCanBeDropped),
		  bCanBeUsed(bCanBeUsed)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UAtomPickupData* PickupData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	int32 Quantity = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bIsStackable = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bIsQuestItem = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bCanBeDropped = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bCanBeUsed = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickedUp, FPickupInfo, PickupInfo, AActor*, Interactor);

UCLASS(Blueprintable, ClassGroup= "Atom")
class ATOMGAMEPLAY_API AAtomPickup : public AAtomInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomPickup();

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UAtomPickupData* PickupData;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	FPickupInfo PickupInfo;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Pickup")
	FOnPickedUp OnPickedUp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, Category = "Atom|Pickup")
	void Pickup(AActor* Interactor);
};
