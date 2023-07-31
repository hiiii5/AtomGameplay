// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AtomDefenseComponent.h"


DEFINE_LOG_CATEGORY(LogAtomDefenseComponent);


// Sets default values for this component's properties
UAtomDefenseComponent::UAtomDefenseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FDamageStats UAtomDefenseComponent::CalculateDamages(FDamageStats DamageStats)
{
	FDamageStats FinalDamages{};

	for (auto Pair : DamageStats.Damages)
	{
		if (!DefenseStats.Resistances.Contains(Pair.Key))
		{
			FinalDamages.Damages.Add(Pair.Key, Pair.Value);
			continue;
		}

		// If the resistance is greater than 1, then it was not normalized by a designer, spit out a warning and normalize it.
		if (DefenseStats.Resistances[Pair.Key] > 1.0f)
		{
			UE_LOG(LogAtomDefenseComponent, Warning, TEXT("UAtomDefenseComponent::CalculateDamages - Resistance for %s is greater than 1.0f, normalizing to 1.0f"), *Pair.Key);
			DefenseStats.Resistances[Pair.Key] = 1.0f;
		}
		// The damage was contained so perform a percentage calculation, and clamp the value so that damage does not become negative(healing)
		float ResistanceDamage = Pair.Value - (Pair.Value * DefenseStats.Resistances[Pair.Key]);
		ResistanceDamage = FMath::Clamp(ResistanceDamage, 0.0f, Pair.Value);
		FinalDamages.Damages.Add(Pair.Key, ResistanceDamage);
	}

	return FinalDamages;
}

float UAtomDefenseComponent::AccumulateDamages(const FDamageStats DamageStats)
{
	// Using a structured binding here as we don't need the full struct, only the value in the map of the struct.
	auto [Damages] = CalculateDamages(DamageStats);
	float TotalDamage = 0.0f;

	for (const auto Pair : Damages)
	{
		TotalDamage += Pair.Value;
	}

	return TotalDamage;
}

bool UAtomDefenseComponent::SetDefenseStats(const FDataTableRowHandle DefenseData)
{
	const FDefenseData* Data = DefenseData.GetRow<FDefenseData>(DefenseData.RowName.ToString());

	if (!Data)
	{
		UE_LOG(LogAtomDefenseComponent, Error, TEXT("UAtomDefenseComponent::SetDefenseStats - Defense data is null"));
		return false;
	}

	DefenseStats = Data->Stats;
	return true;
}

void UAtomDefenseComponent::SetDamageResistance(const FString DamageType, const float Resistance)
{
	DefenseStats.Resistances.Add(DamageType, Resistance);
}

void UAtomDefenseComponent::RemoveDamageResistance(const FString DamageType)
{
	DefenseStats.Resistances.Remove(DamageType);
}

void UAtomDefenseComponent::ClearDamageResistances()
{
	DefenseStats.Resistances.Empty();
}

void UAtomDefenseComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAtomDefenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
