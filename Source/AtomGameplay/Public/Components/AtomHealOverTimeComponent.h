#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtomHealOverTimeComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAtomHealOverTimeComponent, Log, All);


class UAtomHealthComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomHealOverTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomHealOverTimeComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Heal")
	float HealPerTick;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Heal")
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Heal")
	float TickRate = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Heal")
	float InitialDelay = 0.0f;

protected:
	FTimerHandle HealTimerHandle;

	UPROPERTY()
	UAtomHealthComponent* HealthComponent;

	float ElapsedTime = 0.0f;

	UFUNCTION()
	void ApplyHeal();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Heal")
	bool BeginHealOverTime();
};
