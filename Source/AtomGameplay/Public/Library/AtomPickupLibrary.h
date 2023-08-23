// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AtomPickupLibrary.generated.h"

class AAtomPickup;

/**
 * 
 */
UCLASS()
class ATOMGAMEPLAY_API UAtomPickupLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Checks for pickups in the radius and returns an array of all the found pickups ordered by distance from least to greatest
	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory", meta = (WorldContext = "Context"))
	static TArray<AAtomPickup*> CheckForPickupsInRadius(UObject* Context, AActor* Actor, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory", meta = (WorldContext = "Context"))
	static AAtomPickup* GetClosestPickup(UObject* Context, AActor* Actor, float Radius);
	
	UFUNCTION(BlueprintCallable, Category = "Atom|Inventory", meta = (WorldContext = "Context"))
	static bool InteractWithClosestPickup(UObject* Context, AActor* Actor, float Radius);
};
