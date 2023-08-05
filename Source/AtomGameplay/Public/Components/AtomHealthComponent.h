#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stats/AtomStats.h"
#include "AtomHealthComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAtomHealthComponent, Log, All);


/** Delegates used by the health component. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, KilledActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTaken, float, Damage, UDamageType*, DamageType, float, RemainingHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeal, float, Amount, float, Health);


/** Base damage type for processing damage by the health component, and helper components. */
UCLASS(Blueprintable, BlueprintType)
class ATOMGAMEPLAY_API UAtomBaseDamage : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Damage")
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Atom|Health")
	bool bStartWithMaxHealth;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Atom|Health")
	float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Health")
	float MaxHealth;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Health")
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Health")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Health")
	FOnHeal OnHeal;

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	void TakeDamage(float Damage, UAtomBaseDamage* DamageType);

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	void SetHealth(const float NewHealth);

	UFUNCTION(Blueprintable, Category = "Atom|Health")
	void SetMaxHealth(const float NewMaxHealth);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
