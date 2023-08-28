// @AngelGuard


#include "Game/AtomPlayerState.h"

#include "AtomSaveGameSubsystem.h"
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

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("AAtomPlayerState::Save_Implementation() called for %s"), *Pawn->GetClass()->GetName());
	
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

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("AAtomPlayerState::Save_Implementation() found component %s"), *Component->GetClass()->GetName());
		
		FAtomComponentSaveData ComponentSaveData{Component->GetFName().ToString()};

		FMemoryWriter ComponentMemoryWriter(ComponentSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryWriter, true);
		ComponentAr.ArIsSaveGame = true;
		Component->Serialize(ComponentAr);

		PlayerSaveData.Components.Add(ComponentSaveData.ComponentName, ComponentSaveData);
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("AAtomPlayerState::Save_Implementation() found %d components"), PlayerSaveData.Components.Num());
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
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("AAtomPlayerState::Load_Implementation() called"));
	
	if (!SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("AAtomPlayerState::Load_Implementation() called with a nullptr SaveGame!"));
		return;
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("AAtomPlayerState::Load_Implementation() called for %s"), *SaveGame->PlayerSaveData.ActorName);
	
	FMemoryReader MemoryReader(SaveGame->PlayerSaveData.PlayerStateByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
	Ar.ArIsSaveGame = true;
	this->Serialize(Ar);
}
