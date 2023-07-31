﻿#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stats/AtomStats.h"
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
	FDamageStats ResistancesDamage;
};

/** Health component for actors. */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomHealthComponent();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
	bool bStartWithMaxHealth;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Health")
	float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHeal OnHeal;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float Damage, UAtomBaseDamage* DamageType);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetHealth(const float NewHealth);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
