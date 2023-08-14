// @AngelGuard


#include "AtomSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "Game/AtomPlayerState.h"
#include "Game/AtomSaveGame.h"
#include "Interfaces/AtomSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UAtomSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentSlotName = "Default";
	CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::CreateSaveGameObject(UAtomSaveGame::StaticClass()));
}

void UAtomSaveGameSubsystem::HandleStartingPlayer(const AController* Player) const
{
	AAtomPlayerState* PlayerState = Player->GetPlayerState<AAtomPlayerState>();
	if (ensure(PlayerState))
	{
		return;
	}

	PlayerState->Load(CurrentSaveGame);
}

void UAtomSaveGameSubsystem::SetSlotName(FString SlotName)
{
	if (SlotName.Len() == 0)
	{
		return;
	}
	CurrentSlotName = SlotName;
}

FString UAtomSaveGameSubsystem::GetSlotName() const
{
	return CurrentSlotName;
}

void UAtomSaveGameSubsystem::WriteSaveGame() const
{
	if (CurrentSaveGame == nullptr)
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	// strip off the prefix
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	if (AAtomPlayerState* PlayerState = Cast<AAtomPlayerState>(World->GetFirstPlayerController()->PlayerState))
	{
		IAtomSaveInterface::Execute_Save(PlayerState, CurrentSaveGame);
	}

	// TODO Possible low level false flag error here
	FAtomWorldSaveData WorldSaveData = CurrentSaveGame->WorldSaveData.FindOrAdd(MapName);

	// Clear out saved actors from last save.
	WorldSaveData.ActorSaveData.Empty();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<UAtomSaveInterface>())
		{
			continue;
		}

		FAtomActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetFName();
		ActorSaveData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWrite(ActorSaveData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemoryWrite, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);

		IAtomSaveInterface::Execute_Save(Actor, CurrentSaveGame);
		
		CurrentSaveGame->WorldSaveData[MapName].ActorSaveData.Add(ActorSaveData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);
	OnGameSaved.Broadcast(CurrentSaveGame);
}

bool UAtomSaveGameSubsystem::LoadSaveGame(FString SlotName)
{
	// This is ignored if empty
	SetSlotName(SlotName);

	CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::CreateSaveGameObject(UAtomSaveGame::StaticClass()));
		return false;
	}

	FString MapName = GetWorld()->GetMapName();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<UAtomSaveInterface>())
		{
			continue;
		}

		// Find the matching actor and restore its state.
		for (FAtomActorSaveData ActorSaveData : CurrentSaveGame->WorldSaveData[MapName].ActorSaveData)
		{
			if (ActorSaveData.ActorName == Actor->GetFName())
			{
				Actor->SetActorTransform(ActorSaveData.Transform);

				FMemoryReader MemoryReader(ActorSaveData.ByteData);
				FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
				Ar.ArIsSaveGame = true;
				Actor->Serialize(Ar);

				IAtomSaveInterface::Execute_Load(Actor, CurrentSaveGame);
				
				break;
			}
		}
	}

	OnGameLoaded.Broadcast(CurrentSaveGame);

	return true;
}

void UAtomSaveGameSubsystem::OverrideSpawnTransform(APlayerController* NewPlayer) const
{
	if (!IsValid(NewPlayer))
	{
		return;
	}

	const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (!PlayerState)
	{
		return;
	}

	if (APawn* Pawn = PlayerState->GetPawn())
	{
		if (const FAtomPlayerSaveData PlayerSaveData = CurrentSaveGame->PlayerSaveData; PlayerSaveData.bResumeAtTransform)
		{
			Pawn->SetActorTransform(PlayerSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
			NewPlayer->SetControlRotation(PlayerSaveData.Transform.Rotator());
		}
	}
}

