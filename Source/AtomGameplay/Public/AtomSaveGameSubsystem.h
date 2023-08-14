// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AtomSaveGameSubsystem.generated.h"

class UAtomSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGame, UAtomSaveGame*, SaveGame);

/**
 * 
 */
UCLASS(meta=(DisplayName="Atom Save Game Subsystem"))
class ATOMGAMEPLAY_API UAtomSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	FString CurrentSlotName;

	UPROPERTY()
	TObjectPtr<UAtomSaveGame> CurrentSaveGame;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/**
	 * @brief Restores serialized data from a players state.
	 * @param Player Player that is starting.
	 */
	void HandleStartingPlayer(const AController* Player) const;

	/**
	 * @brief Sets the slot name for the save game.
	 * @param SlotName Name of the slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Atom|SaveGame")
	void SetSlotName(FString SlotName);

	/**
	 * @brief Gets the slot name for the save game.
	 * @return Name of the slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Atom|SaveGame")
	FString GetSlotName() const;

	/**
	 * @brief Writes the save game to disk.
	 */
	UFUNCTION(BlueprintCallable, Category = "Atom|SaveGame")
	void WriteSaveGame() const;

	/**
	 * @brief Loads the save game from disk. This occurs when a map is loaded with the "SaveGame" option. Ex: "Open Level" node Options param "SaveGame={NameOfSlotWithoutBraces}.
	 * @param SlotName {OPTIONAL} Name of the slot.
	 * @note It is a better practice to use the open level node as described above.
	 */
	UFUNCTION(BlueprintCallable, Category = "Atom|SaveGame")
	bool LoadSaveGame(FString SlotName = "");
	
	void OverrideSpawnTransform(APlayerController* NewPlayer) const;

	/** Delegate that is fired when the game is saved. */
	UPROPERTY(BlueprintAssignable, Category = "Atom|SaveGame")
	FOnSaveGame OnGameSaved;

	/** Delegate that is fired when the game is loaded. */
	UPROPERTY(BlueprintAssignable, Category = "Atom|SaveGame")
	FOnSaveGame OnGameLoaded;
};