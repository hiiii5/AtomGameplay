// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomPickupData.h"
#include "Stats/AtomStats.h"
#include "AtomWeaponData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ATOMGAMEPLAY_API UAtomWeaponData : public UAtomPickupData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	UStaticMesh* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	USoundBase* FireSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	USoundBase* ReloadSound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	FDamageStats DamageTypes;
};
