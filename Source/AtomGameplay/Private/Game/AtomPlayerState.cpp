// @AngelGuard


#include "Game/AtomPlayerState.h"

#include "Game/AtomSaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AAtomPlayerState::Save_Implementation(UAtomSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Save_Implementation() called with a nullptr SaveGame!"));
		return;
	}

	APawn* Pawn = GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Save_Implementation() called with a nullptr Pawn!"));
		return;
	}

	FAtomPlayerSaveData PlayerSaveData{};
	PlayerSaveData.ActorName = Pawn->GetClass()->GetName();
	PlayerSaveData.PlayerId = GetUniqueId().ToString();
	PlayerSaveData.bResumeAtTransform = true;

	// Serialize the components
	for (UActorComponent* Component : Pawn->GetComponents())
	{
		if (!Component->Implements<UAtomSaveInterface>())
		{
			continue;
		}

		FAtomComponentSaveData ComponentSaveData{Component->GetFName().ToString()};

		FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryWriter, true);
		ComponentAr.ArIsSaveGame = true;
		Component->Serialize(ComponentAr);

		PlayerSaveData.Components.Add(ComponentSaveData.ComponentName, ComponentSaveData);
	}

	// Serialize the pawn
	FMemoryWriter PawnMemoryWriter(PlayerSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive PawnAr(PawnMemoryWriter, true);
	PawnAr.ArIsSaveGame = true;
	Pawn->Serialize(PawnAr);

	PlayerSaveData.Transform = Pawn->GetActorTransform();

	// Serialize the state.
	FMemoryWriter MemoryWrite(PlayerSaveData.PlayerStateByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemoryWrite, true);
	Ar.ArIsSaveGame = true;
	this->Serialize(Ar);

	SaveGame->PlayerSaveData = PlayerSaveData;
}

void AAtomPlayerState::Load_Implementation(UAtomSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Load_Implementation() called with a nullptr SaveGame!"));
		return;
	}

	FMemoryReader MemoryReader(SaveGame->PlayerSaveData.PlayerStateByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
	Ar.ArIsSaveGame = true;
	this->Serialize(Ar);
}
