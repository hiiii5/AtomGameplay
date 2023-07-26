#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtomDamageOverTimeComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAtomDamageOverTimeComponent, Log, All);


class UAtomBaseDamage;
class UAtomHealthComponent;


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomDamageOverTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomDamageOverTimeComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float DamagePerTick;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float TickRate = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	float InitialDelay = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage")
	UAtomBaseDamage* DamageType;
	
protected:
	FTimerHandle DamageTimerHandle;

	UPROPERTY()
	UAtomHealthComponent* HealthComponent;

	float ElapsedTime = 0.0f;

	UFUNCTION()
	void ApplyDamage();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool BeginDamageOverTime();
};
