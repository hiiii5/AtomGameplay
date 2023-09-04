// @AngelGuard


#include "AtomSaveGameSubsystem.h"

#include "EngineUtils.h"
#include "Game/AtomPlayerState.h"
#include "Game/AtomSaveGame.h"
#include "Interfaces/AtomSaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY(LogAtomSaveSubsystem);

void UAtomSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CurrentSlotName = "Default";
	CurrentSaveGame = Cast<UAtomSaveGame>(UGameplayStatics::CreateSaveGameObject(UAtomSaveGame::StaticClass()));
}

void UAtomSaveGameSubsystem::HandleStartingPlayer(const AController* Player) const
{
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::HandleStartingPlayer"));
	
	AAtomPlayerState* PlayerState = Player->GetPlayerState<AAtomPlayerState>();

	if (!PlayerState)
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::HandleStartingPlayer no player state"));
		return;
	}
	
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::HandleStartingPlayer - PlayerState: %s"), *PlayerState->GetPlayerName());
	IAtomSaveInterface::Execute_Load(PlayerState, CurrentSaveGame);
}

void UAtomSaveGameSubsystem::SetSlotName(FString SlotName)
{
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::SetSlotName - %s"), *SlotName);
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

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame begin - %s"), *CurrentSlotName);
	
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

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame - %s"), *MapName);
	
	// Save all actors that implement the save game interface.
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<UAtomSaveInterface>() || Actor == PlayerState || Actor->ActorHasTag(FName("Player")))
		{
			continue;
		}

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame - %s"), *Actor->GetActorNameOrLabel());
		
		// Create the actor save data here to save components into it.
		FAtomActorSaveData ActorSaveData{Actor->GetActorNameOrLabel(), Actor->GetActorTransform()};
		ActorSaveData.ActorClass = Actor->GetClass()->GetClassPathName().ToString();

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame saved actor class - %s"), *ActorSaveData.ActorClass);
		
		// Serialize that actor from inside out, starting with the components.
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component->Implements<UAtomSaveInterface>())
			{
				continue;
			}

			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame - %s"), *Component->GetFName().ToString());
			
			FAtomComponentSaveData ComponentSaveData{Component->GetFName().ToString()};
			
			FMemoryWriter ComponentMemoryWrite(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryWrite, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			ActorSaveData.Components.Add(ComponentSaveData.ComponentName, ComponentSaveData);
			
			IAtomSaveInterface::Execute_Save(Component, CurrentSaveGame);
		}

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame finished - %s"), *ActorSaveData.ActorName);
		
		// Now serialize the actor itself.
		FMemoryWriter MemoryWrite(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemoryWrite, true);
		Ar.ArIsSaveGame = 1;
		Ar.ArSerializingDefaults = 1;
		Ar.StartSerializingDefaults();
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

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::WriteSaveGame - %s"), *CurrentSaveGame->CurrentLevelName);
	
	CurrentSaveGame->CurrentLevelName = MapName;
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);
	OnGameSaved.Broadcast(CurrentSaveGame);
}

bool UAtomSaveGameSubsystem::LoadSaveGame(FString SlotName)
{
	
	// This is ignored if empty
	SetSlotName(SlotName);
		
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame start - %s"), *CurrentSlotName);
	
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
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame - %s"), *MapName);
		return false;
	}
	
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame - %s"), *MapName);
	
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

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loading actor - %s"), *Actor->GetActorNameOrLabel());
    	
		// The actor was in the world but not in the save, so therefore it was destroyed last session.
    	if (!CurrentSaveGame->WorldSaveData[MapName].SavedActors.Contains(Actor->GetActorNameOrLabel()))
        {
    		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame destroying actor - %s"), *Actor->GetActorNameOrLabel());
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

			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loading component - %s"), *Component->GetFName().ToString());
			
			FString ComponentName = Component->GetFName().ToString();
			// If the component was not in the save then it was destroyed last session.
			if (!ActorSaveData.Components.Contains(ComponentName))
			{
				UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame destroying component - %s"), *Component->GetFName().ToString());
				Component->DestroyComponent();
				continue;
			}

			// Load the component.
			FAtomComponentSaveData ComponentSaveData = ActorSaveData.Components[ComponentName];
			FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loaded component - %s"), *Component->GetFName().ToString());
			
			IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
		}
    	
    	FMemoryReader MemoryReader(ActorSaveData.ByteData);
        FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
        Ar.ArIsSaveGame = true;
        Actor->Serialize(Ar);

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loaded actor - %s"), *Actor->GetActorNameOrLabel());
    	
		Actor->SetActorTransform(ActorSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
    	
        IAtomSaveInterface::Execute_Load(Actor, CurrentSaveGame);
    	LoadedActors.Add(ActorName);
    }

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loading unloaded actors - %d"), LoadedActors.Num());
	
	// Now we need to spawn the actors that were not in the world but were in the save.
	for (TTuple<FString, FAtomActorSaveData> Tuple : CurrentSaveGame->WorldSaveData[MapName].SavedActors)
	{
		if (LoadedActors.Contains(Tuple.Key))
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame skipping actor - %s"), *Tuple.Key);
			continue;
		}

		// Spawn the actor.
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Name = FName(*Tuple.Key);
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame spawning actor - %s"), *Tuple.Key);

		if (*Tuple.Value.ActorClass == nullptr)
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame actor class is null - %s"), *Tuple.Key);
			continue;
		}

		// If the actor to spawn was not found in the world context then there is a crash, ensure that the saved actor from this checkpoint was in the primary level.
		// find object with this package and name
		UClass* ActorClass = FindObject<UClass>(World, *Tuple.Value.ActorClass);

		if (!ActorClass)
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame actor class - %s"), *ActorClass->GetName());
			continue;
		}
		
		AActor* Actor = World->SpawnActor<AActor>(ActorClass, Tuple.Value.Transform, SpawnParameters);

		if (!Actor)
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame failed to spawn actor - %s"), *Tuple.Key);
			continue;
		}

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame spawned actor - %s"), *Tuple.Key);
		
		// Deserialize the actors components.
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (!Component->Implements<UAtomSaveInterface>())
			{
				continue;
			}

			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loading component - %s"), *Component->GetFName().ToString());
			
			FString ComponentName = Component->GetFName().ToString();
			// If the component was not in the save then it was destroyed last session.
			if (!Tuple.Value.Components.Contains(ComponentName))
			{
				UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame destroying component - %s"), *Component->GetFName().ToString());
				Component->DestroyComponent();
				continue;
			}

			// Load the component.
			FAtomComponentSaveData ComponentSaveData = Tuple.Value.Components[ComponentName];
			FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
			FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
			ComponentAr.ArIsSaveGame = true;
			Component->Serialize(ComponentAr);

			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame loaded component - %s"), *Component->GetFName().ToString());
			
			IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
		}
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadSaveGame end - %s"), *CurrentSlotName);
	
	OnGameLoaded.Broadcast(CurrentSaveGame);

	return true;
}

void UAtomSaveGameSubsystem::OverrideSpawnTransform(APlayerController* NewPlayer) const
{
	if (!IsValid(NewPlayer))
	{
		return;
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::OverrideSpawnTransform"));
	
	// Check that the current world matches the current world in the save
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	if (!CurrentSaveGame->CurrentLevelName.Equals(MapName))
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::OverrideSpawnTransform not on current level from save - %s"), *CurrentSaveGame->CurrentLevelName);
		return;
	}

	const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::OverrideSpawnTransform no player state"));
		return;
	}

	if (APawn* Pawn = PlayerState->GetOwningController()->GetPawn())
	{
		if (const FAtomPlayerSaveData PlayerSaveData = CurrentSaveGame->PlayerSaveData; PlayerSaveData.bResumeAtTransform)
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::OverrideSpawnTransform - %s"), *Pawn->GetActorNameOrLabel());
			Pawn->SetActorTransform(PlayerSaveData.Transform, false, nullptr, ETeleportType::TeleportPhysics);
			NewPlayer->SetControlRotation(PlayerSaveData.Transform.Rotator());
		}
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::OverrideSpawnTransform end"));
}

void UAtomSaveGameSubsystem::LoadPlayerPawn(APlayerController* NewPlayer) const
{
	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn"));
	if (!HasValidSaveGame() || !CurrentSaveGame)
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn no valid save game"));
		return;
	}

	if (CurrentSaveGame->PlayerSaveData.PlayerId.IsEmpty())
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn no player id"));
		return;
	}
	
	// Check that the current world matches the current world in the save
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GameInstance->GetWorld();
	FString MapName = World->GetMapName();
	MapName.RemoveFromStart(World->StreamingLevelsPrefix);
	if (!CurrentSaveGame->CurrentLevelName.Equals(MapName))
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn not on current level from save - %s"), *CurrentSaveGame->CurrentLevelName);
		return;
	}
	
	APawn *Pawn = NewPlayer->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn no pawn"));
		return;
	}

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn - %s"), *Pawn->GetActorNameOrLabel());
	
	// Deserialize the pawns components.
	for (UActorComponent* Component : Pawn->GetComponents())
	{
		if (!Component->Implements<UAtomSaveInterface>())
		{
			continue;
		}

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn loading component - %s"), *Component->GetFName().ToString());
		
		FString ComponentName = Component->GetFName().ToString();
		// If the component was not in the save then it was destroyed last session.
		if (!CurrentSaveGame->PlayerSaveData.Components.Contains(ComponentName))
		{
			UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn destroying component - %s"), *Component->GetFName().ToString());	
			Component->DestroyComponent();
			continue;
		}

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn found component - %s"), *Component->GetFName().ToString());
		
		// Load the component.
		FAtomComponentSaveData ComponentSaveData = CurrentSaveGame->PlayerSaveData.Components[ComponentName];
		FMemoryReader ComponentMemoryReader(ComponentSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive ComponentAr(ComponentMemoryReader, true);
		ComponentAr.ArIsSaveGame = true;
		Component->Serialize(ComponentAr);

		UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn loaded component - %s"), *Component->GetFName().ToString());

		IAtomSaveInterface::Execute_Load(Component, CurrentSaveGame);
	}

	// Deserialize the pawn.
	FMemoryReader PawnMemoryReader(CurrentSaveGame->PlayerSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive PawnAr(PawnMemoryReader, true);
	PawnAr.ArIsSaveGame = true;
	Pawn->Serialize(PawnAr);

	UE_LOG(LogAtomSaveSubsystem, Log, TEXT("UAtomSaveGameSubsystem::LoadPlayerPawn loaded pawn - %s"), *Pawn->GetActorNameOrLabel());
	
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

