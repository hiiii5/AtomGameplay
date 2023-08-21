// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AtomSaveGame.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAtomComponentSaveData
{
	GENERATED_BODY()

	FAtomComponentSaveData() : ComponentName(""), ByteData(TArray<uint8>()) {}
	FAtomComponentSaveData(const FString &ComponentName) : ComponentName(ComponentName), ByteData(TArray<uint8>()) {}

	explicit FAtomComponentSaveData(const FString& InComponentName, const TArray<uint8>& InByteData)
		: ComponentName(InComponentName), ByteData(InByteData) {}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString ComponentName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TArray<uint8> ByteData;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAtomActorSaveData
{
	GENERATED_BODY()

	FAtomActorSaveData() : ActorName(""), ActorClass(""), Transform(FTransform::Identity), ByteData(TArray<uint8>()), Components(TMap<FString, FAtomComponentSaveData>()) {}
	FAtomActorSaveData(const FString &ActorName, const FTransform &Transform) : ActorName(ActorName), ActorClass(""), Transform(Transform), ByteData(TArray<uint8>()), Components(TMap<FString, FAtomComponentSaveData>()) {}
	
	explicit FAtomActorSaveData(const FString& ActorName, const FString& ActorClass, const FTransform& Transform, const TArray<uint8>& ByteData, const TMap<FString, FAtomComponentSaveData>& Components)
		: ActorName(ActorName), ActorClass(ActorClass), Transform(Transform), ByteData(ByteData), Components(Components) {}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString ActorName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString ActorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FTransform Transform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TArray<uint8> ByteData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TMap<FString, FAtomComponentSaveData> Components;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAtomWorldSaveData
{
	GENERATED_BODY()

	FAtomWorldSaveData() : LevelName(""), SavedActors(TMap<FString, FAtomActorSaveData>()) {}
	FAtomWorldSaveData(const FString &LevelName) : LevelName(LevelName), SavedActors(TMap<FString, FAtomActorSaveData>()) {}

	explicit FAtomWorldSaveData(const FString& InLevelName, const TMap<FString, FAtomActorSaveData>& InSavedActors)
		: LevelName(InLevelName), SavedActors(InSavedActors) {}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString LevelName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TMap<FString, FAtomActorSaveData> SavedActors;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAtomPlayerSaveData : public FAtomActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString PlayerId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	bool bResumeAtTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TArray<uint8> PlayerStateByteData;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ATOMGAMEPLAY_API UAtomSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TMap<FString, FAtomWorldSaveData> WorldSaveData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString CurrentLevelName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FAtomPlayerSaveData PlayerSaveData;
};
