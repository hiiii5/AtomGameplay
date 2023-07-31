// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AtomStats.generated.h"

USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDefenseStats
{
	GENERATED_BODY()

public:
	/** Mutable, percentage based, stats for resistances. Example: Armor 25%, blunt 5%, poison 100%. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
	TMap<FString, float> Resistances;
};

USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDamageStats
{
	GENERATED_BODY()

public:
	/** Mutable, number based, stats for damages. Example: Armor 150.0f, blunt 25.2f, poison 10.55f. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	TMap<FString, float> Damages;
};

/** Helper for data table based data. */
USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDefenseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
	FDefenseStats Stats;
};

/** Helper for data table based data. */
USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDamageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	FDamageStats Stats;
};
