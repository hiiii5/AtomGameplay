// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AtomPickupData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "Atom")
class ATOMGAMEPLAY_API UAtomPickupData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	int32 MaxStackQuantity = 1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bIsStackable = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bIsQuestItem = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bCanBeDropped = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	bool bCanBeUsed = false;
	
	// This is the implementation class of the pickup actor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UClass* PickupClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UClass* InventoryItemClass;
};
