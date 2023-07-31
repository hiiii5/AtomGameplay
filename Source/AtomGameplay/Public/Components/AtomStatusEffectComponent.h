// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AtomStatusEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectTicked, const FString&, StatusEffectName);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomStatusEffectComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StatusEffect")
	TMap<FString, float> StatusEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StatusEffect")
	float DecayPerTick = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StatusEffect")
	float TickRate = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StatusEffect")
	float InitialDelay = 0.0f;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffect")
	FOnStatusEffectTicked OnStatusEffectTicked;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FTimerHandle StatusEffectTimerHandle;
	
	UFUNCTION()
	void OnStatusEffectTick();

	UFUNCTION()
	void BeginStatusEffectTimer();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Accumulates the amount of the status effect to the current value. If this is the first status effect a timer will be started, otherwise the effect will be reduced on the next timer tick.
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	void AccumulateStatusEffect(const FString StatusEffectName, const float StatusEffectValue);

	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	void RemoveStatusEffect(const FString StatusEffectName);

	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	void GetStatusEffect(const FString StatusEffectName, float& StatusEffectValue);
};
