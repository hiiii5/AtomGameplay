// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AtomSaveInterface.generated.h"

class UAtomSaveGame;
// This class does not need to be modified.
UINTERFACE()
class UAtomSaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATOMGAMEPLAY_API IAtomSaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Save")
	void Save(UAtomSaveGame* SaveGame);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Save")
	void Load(UAtomSaveGame* SaveGame);
};
