// @AngelGuard


#include "Game/AtomPlayerState.h"

#include "Game/AtomSaveGame.h"

void AAtomPlayerState::Save_Implementation(UAtomSaveGame* SaveGame)
{
	if(!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Save_Implementation() called with a nullptr SaveGame!"));
		return;
	}
	
	FAtomPlayerSaveData PlayerSaveData;
	PlayerSaveData.ActorName = GetFName();
	PlayerSaveData.PlayerId = GetUniqueId().ToString();
	PlayerSaveData.bResumeAtTransform = true;

	if (const APawn *Pawn = GetPawn())
	{
		PlayerSaveData.Transform = Pawn->GetActorTransform();
	}

	SaveGame->PlayerSaveData = PlayerSaveData;
}

void AAtomPlayerState::Load_Implementation(UAtomSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Load_Implementation() called with a nullptr SaveGame!"));
		return;
	}

	// Could do more here...
}
