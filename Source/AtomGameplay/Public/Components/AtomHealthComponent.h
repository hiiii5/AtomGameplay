#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "AtomHealthComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAtomHealthComponent, Log, All);


/** Delegates used by the health component. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, Damage, UDamageType*, DamageType, float, RemainingHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeal, float, Amount, float, Health);


/** Base damage type for processing damage by the health component, and helper components. */
UCLASS(Blueprintable, BlueprintType)
class ATOMGAMEPLAY_API UAtomBaseDamage : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float ArmorDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	TMap<FString, float> ResistancesDamage;
};


USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDefenseData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
	float Armor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
	TMap<FString, float> Resistances;
};


USTRUCT(BlueprintType)
struct ATOMGAMEPLAY_API FDamageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float ArmorDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	TMap<FString, float> ResistancesDamage;
};


/** Health component for actors. */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomHealthComponent();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	FDataTableRowHandle DefenseData;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHeal OnHeal;

	/** Overload this to provide a custom damage calculation. The base version just sums up the results of each resistance damage, armor damage, and the base damage. */
	UFUNCTION(BlueprintNativeEvent, Category="Health")
	float CalculateDamage(float Damage, const UAtomBaseDamage* DamageType);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float Damage, UAtomBaseDamage* DamageType);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float Amount);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="Health")	
	float GetHealth() const;
};
