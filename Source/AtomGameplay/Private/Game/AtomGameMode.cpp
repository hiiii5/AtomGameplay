// @AngelGuard


#include "Game/AtomGameMode.h"

#include "AtomSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

void AAtomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UAtomSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<UAtomSaveGameSubsystem>();
	FString SlotName = UGameplayStatics::ParseOption(Options, "SaveGame");

	// Optionally load the world based on options passed to a load command
	SaveGameSubsystem->LoadSaveGame(SlotName);
}

void AAtomGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	UAtomSaveGameSubsystem* SaveGameSubsystem = GetGameInstance()->GetSubsystem<UAtomSaveGameSubsystem>();
	SaveGameSubsystem->HandleStartingPlayer(NewPlayer);
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	SaveGameSubsystem->OverrideSpawnTransform(NewPlayer);
}
