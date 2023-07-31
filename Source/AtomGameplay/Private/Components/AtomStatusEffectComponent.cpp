// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AtomStatusEffectComponent.h"


// Sets default values for this component's properties
UAtomStatusEffectComponent::UAtomStatusEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Prepare the status effects map.
	StatusEffects = TMap<FString, float>();
}


// Called when the game starts
void UAtomStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAtomStatusEffectComponent::OnStatusEffectTick()
{
	// First we need to check if there is any status effects to decay, if not then we can clear the timer.
	if (StatusEffects.Num() > 0)
	{
		// Loop through all the status effects and decay them.
		for (auto& StatusEffect : StatusEffects)
		{
			OnStatusEffectTicked.Broadcast(StatusEffect.Key);
			StatusEffect.Value -= DecayPerTick;

			if (StatusEffect.Value <= 0.0f)
			{
				StatusEffects.Remove(StatusEffect.Key);
			}
		}
	}
	else
	{
		// Clear the timer if there are no status effects.
		GetWorld()->GetTimerManager().ClearTimer(StatusEffectTimerHandle);
	}
}

void UAtomStatusEffectComponent::BeginStatusEffectTimer()
{
	// Before we create a timer, we need to check if one is already ticking so that we don't lose the original timer handle.
	if (!GetWorld()->GetTimerManager().IsTimerActive(StatusEffectTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(StatusEffectTimerHandle, this, &UAtomStatusEffectComponent::OnStatusEffectTick, TickRate, true, InitialDelay);
	}
}


// Called every frame
void UAtomStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAtomStatusEffectComponent::AccumulateStatusEffect(const FString StatusEffectName, const float StatusEffectValue)
{
	// Add the amount to an existing effect or create a new one.
	if (StatusEffects.Contains(StatusEffectName))
	{
		StatusEffects[StatusEffectName] += StatusEffectValue;
	}
	else
	{
		StatusEffects.Add(StatusEffectName, StatusEffectValue);
	}
}

void UAtomStatusEffectComponent::RemoveStatusEffect(const FString StatusEffectName)
{
	StatusEffects.Remove(StatusEffectName);
}

void UAtomStatusEffectComponent::GetStatusEffect(const FString StatusEffectName, float& StatusEffectValue)
{
	StatusEffectValue = 0.0f;
	if (StatusEffects.Contains(StatusEffectName))
	{
		StatusEffectValue = StatusEffects[StatusEffectName];
	}
}

