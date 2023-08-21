// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AtomPickupData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "Atom")
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
	UClass* PickupClass;
};
