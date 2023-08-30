// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AtomPickupInterface.generated.h"

struct FAtomPickupInfo;
// This class does not need to be modified.
UINTERFACE()
class UAtomPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATOMGAMEPLAY_API IAtomPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Pickup")
	void Pickup(FAtomPickupInfo PickupInfo, AActor* Interactor);
};
