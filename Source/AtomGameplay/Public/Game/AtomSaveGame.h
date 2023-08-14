// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AtomSaveGame.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FAtomActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FName ActorName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FTransform Transform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TArray<uint8> ByteData;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAtomWorldSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	TArray<FAtomActorSaveData> ActorSaveData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString LevelName;
};

USTRUCT(Blueprintable, BlueprintType)
struct FAtomPlayerSaveData : public FAtomActorSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	FString PlayerId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|SaveGame")
	bool bResumeAtTransform;
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
	FAtomPlayerSaveData PlayerSaveData;
};
