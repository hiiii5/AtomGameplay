// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/AtomSaveInterface.h"
#include "AtomPlayerState.generated.h"

// Forward declaration for delegates using the class
class AAtomPlayerState;
class UAtomSaveGame;


/**
 * 
 */
UCLASS()
class ATOMGAMEPLAY_API AAtomPlayerState : public APlayerState, public IAtomSaveInterface
{
	GENERATED_BODY()

public:
	virtual void Save_Implementation(UAtomSaveGame* SaveGame) override;
	virtual void Load_Implementation(UAtomSaveGame* SaveGame) override;
};
