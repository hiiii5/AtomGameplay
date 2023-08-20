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

	IAtomSaveInterface::Execute_Load(PlayerState, CurrentSaveGame);
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

bool UAtomSaveGameSubsystem::HasValidSaveGame() const
{
	return CurrentSaveGame != nullptr;
}

void UAtomSaveGameSubsystem::WriteSaveGame() const
{
	if (!HasValidSaveGame())
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();

	APlayerController* PlayerController = World->GetFirstPlayerController();
	ensure(PlayerController);
	
	APlayerState* PlayerState = World->GetFirstPlayerController()->PlayerState;
	ensure(PlayerState);
	
	IAtomSaveInterface::Execute_Save(PlayerState, CurrentSaveGame);

	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	FAtomWorldSaveData WorldSaveData{MapName};
	
	// Save all actors that implement the save game interface.
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<UAtomSaveInterface>() || Actor == PlayerState || Actor->ActorHasTag(FName("Player")))
		{
			continue;
		}

		// Create the actor save data here to save components into it.
		FAtomActorSaveData ActorSaveData{Actor->GetActorNameOrLabel(), Actor->GetActorTransform()};
		ActorSaveData.ActorClass = Actor->GetClass()->GetClassPathName().ToString();

		// Serialize that actor from inside out, starting with the components.
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component->Implements<UAtomSaveInterface>())
			{
				continue;
			}
			
			FAtomComponentSaveData ComponentSaveData{Component->GetFName().ToString()};
			
			FMemoryWriter ComponentMemoryWrite(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryWrite, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			ActorSaveData.Components.Add(ComponentSaveData.ComponentName, ComponentSaveData);
			
			IAtomSaveInterface::Execute_Save(Component, CurrentSaveGame);
		}

		// Now serialize the actor itself.
		FMemoryWriter MemoryWrite(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemoryWrite, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
		IAtomSaveInterface::Execute_Save(Actor, CurrentSaveGame);

		WorldSaveData.SavedActors.Add(ActorSaveData.ActorName, ActorSaveData);
	}

	// Overwrite the world if it exists, otherwise add it.
	if (CurrentSaveGame->WorldSaveData.Contains(MapName))
	{
		CurrentSaveGame->WorldSaveData[MapName] = WorldSaveData;
	}
	else
	{
		CurrentSaveGame->WorldSaveData.Add(MapName, WorldSaveData);
	}

	CurrentSaveGame->CurrentLevelName = MapName;
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);
	OnGameSaved.Broadcast(CurrentSaveGame);
}

bool UAtomSaveGameSubsystem::LoadSaveGame(FString SlotName)
{
	// This is ignored if empty
	SetSlotName(SlotName);

	// Ensure there is a valid save game.
	CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::CreateSaveGameObject(UAtomSaveGame::StaticClass()));
		return false;
	}

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	// Escape out early if for whatever reason the map name is not in the save.
	if (!CurrentSaveGame->WorldSaveData.Contains(MapName))
	{
		return false;
	}
	
	// We will store every actor that was successfully loaded in this array to then spawn the remaining actors in the save.
	// These would be actors that were spawned after the start of the world.
	TArray<FString> LoadedActors{};
	// Iterate over the actors in the world.
    for (FActorIterator It(World); It; ++It)
    {
		AActor* Actor = *It;
    	
		// Ensure our actor was a saveable actor. At this point the player state is not in the world.
		if (!IsValid(Actor) || !Actor->Implements<UAtomSaveInterface>() || Actor->ActorHasTag(FName("Player")))
		{
			continue;
		}
    	
		// The actor was in the world but not in the save, so therefore it was destroyed last session.
    	if (!CurrentSaveGame->WorldSaveData[MapName].SavedActors.Contains(Actor->GetActorNameOrLabel()))
        {
        	Actor->Destroy();
    		continue;
        }

		FString ActorName = Actor->GetActorNameOrLabel();
		FAtomActorSaveData ActorSaveData = CurrentSaveGame->WorldSaveData[MapName].SavedActors[ActorName];
    	
		// Since we save components and then the actor we need to load the components and then the actor.
    	// This prevents the references to the components from being invalid.
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component->Implements<UAtomSaveInterface>())
			{
				continue;
			}

			FString ComponentName = Component->GetFName().ToString();
			// If the component was not in the save then it was destroyed last session.
			if (!ActorSaveData.Components.Contains(ComponentName))
			{
				Component->DestroyComponent();
				continue;
			}

			// Load the component.
			FAtomComponentSaveData ComponentSaveData = ActorSaveData.Components[ComponentName];
			FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
		}
    	
    	FMemoryReader MemoryReader(ActorSaveData.ByteData);
        FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
        Ar.ArIsSaveGame = true;
        Actor->Serialize(Ar);

		Actor->SetActorTransform(ActorSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
    	
        IAtomSaveInterface::Execute_Load(Actor, CurrentSaveGame);
    	LoadedActors.Add(ActorName);
    }

	// Now we need to spawn the actors that were not in the world but were in the save.
	for (TTuple<FString, FAtomActorSaveData> Tuple : CurrentSaveGame->WorldSaveData[MapName].SavedActors)
	{
		if (LoadedActors.Contains(Tuple.Key))
		{
			continue;
		}

		// Spawn the actor.
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Name = FName(*Tuple.Key);
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// TODO Fix deprecated function.
		UClass* ActorClass = FindObject<UClass>(ANY_PACKAGE, *Tuple.Value.ActorClass);
		AActor* Actor = World->SpawnActor<AActor>(ActorClass, Tuple.Value.Transform, SpawnParameters);

		// Deserialize the actors components.
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component->Implements<UAtomSaveInterface>())
			{
				continue;
			}

			FString ComponentName = Component->GetFName().ToString();
			// If the component was not in the save then it was destroyed last session.
			if (!Tuple.Value.Components.Contains(ComponentName))
			{
				Component->DestroyComponent();
				continue;
			}

			// Load the component.
			FAtomComponentSaveData ComponentSaveData = Tuple.Value.Components[ComponentName];
			FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
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

	// Check that the current world matches the current world in the save
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	if (!CurrentSaveGame->CurrentLevelName.Equals(MapName))
	{
		return;
	}

	const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (!PlayerState)
	{
		return;
	}

	if (APawn* Pawn = PlayerState->GetOwningController()->GetPawn())
	{
		if (const FAtomPlayerSaveData PlayerSaveData = CurrentSaveGame->PlayerSaveData; PlayerSaveData.bResumeAtTransform)
		{
			Pawn->SetActorTransform(PlayerSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
			NewPlayer->SetControlRotation(PlayerSaveData.Transform.Rotator());

		}
	}
}

void UAtomSaveGameSubsystem::LoadPlayerPawn(APlayerController* NewPlayer) const
{
	if (!HasValidSaveGame() || !CurrentSaveGame)
	{
		return;
	}

	if (CurrentSaveGame->PlayerSaveData.PlayerId.IsEmpty())
	{
		return;
	}

	// Check that the current world matches the current world in the save
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	if (!CurrentSaveGame->CurrentLevelName.Equals(MapName))
	{
		return;
	}
	
	APawn *Pawn = NewPlayer->GetPawn();
	if (!Pawn)
	{
		return;
	}
	
	// Deserialize the pawns components.
	for (UActorComponent* Component : Pawn->GetComponents())
	{
		if (!Component->Implements<UAtomSaveInterface>())
		{
			continue;
		}

		FString ComponentName = Component->GetFName().ToString();
		// If the component was not in the save then it was destroyed last session.
		if (!CurrentSaveGame->PlayerSaveData.Components.Contains(ComponentName))
		{
			Component->DestroyComponent();
			continue;
		}

		// Load the component.
		FAtomComponentSaveData ComponentSaveData = CurrentSaveGame->PlayerSaveData.Components[ComponentName];
		FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
		ComponentAr.ArIsSaveGame = true;
		Component->Serialize(ComponentAr);

		IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
	}

	// Deserialize the pawn.
	FMemoryReader PawnMemoryReader(CurrentSaveGame->PlayerSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive PawnAr(PawnMemoryReader, true);
	PawnAr.ArIsSaveGame = true;
	Pawn->Serialize(PawnAr);
	
	IAtomSaveInterface::Execute_Load(Pawn, CurrentSaveGame);
}

FString UAtomSaveGameSubsystem::GetSavedMapName(FString SlotName) const
{
	// If the slot name is empty then use the default slot name.
	if (SlotName.IsEmpty())
	{
		SlotName = "Default";
	}

	// Check that the save game exists
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return "";
	}

	// Load the save game.
	UAtomSaveGame* SaveGame = Cast<UAtomSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!IsValid(SaveGame))
	{
		return "";
	}

	return SaveGame->CurrentLevelName;
}

