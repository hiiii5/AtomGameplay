#include "../../Public/Components/AtomHealthComponent.h"


// Sets default values for this component's properties
UAtomHealthComponent::UAtomHealthComponent()
{
	// The health component does not perform on tick calculations. These can be performed by helper components.
	PrimaryComponentTick.bCanEverTick = false;
}

float UAtomHealthComponent::CalculateDamage_Implementation(const float Damage, const UAtomBaseDamage* DamageType)
{
	// A base formula will be used, first we will find matching resistances and get their totals
	// Then we will calculate the armor damage
	// Then we will add the base damage and the damage from the resistances and armor that is remaining as the total damage
	TArray<float> ResistanceDamages{};


	FDefenseData* Data = nullptr; 

	// We want to guard against a null reference in case the health component didn't have any defense data setup in editor.
	if (DefenseData.IsNull())
	{
		// Add all the damage types to the resistance damages array.
		for (auto ResistanceType : DamageType->ResistancesDamage)
		{
			ResistanceDamages.Add(ResistanceType.Value);
		}
	}
	else
	{
		Data = DefenseData.GetRow<FDefenseData>(DefenseData.RowName.ToString());
		
		for (auto ResistanceType : DamageType->ResistancesDamage)
		{
			if (!Data)
			{
				ResistanceDamages.Add(ResistanceType.Value);
			}

			if (Data->Resistances.Contains(ResistanceType.Key))
			{
				float ResistanceDamage = ResistanceType.Value - Data->Resistances[ResistanceType.Key];

				// Don't let negative values go past this point as that would be healing.			
				ResistanceDamage = FMath::Clamp(ResistanceDamage, 0.0f, ResistanceType.Value);
				ResistanceDamages.Add(ResistanceDamage);
			}
			else
			{
				ResistanceDamages.Add(ResistanceType.Value);
			}
		}
	}

	// Now lets calculate that armor damage
	float ArmorDamage = DamageType->ArmorDamage;
	if(Data != nullptr)
	{
		 ArmorDamage -= Data->Armor;
	}
	ArmorDamage = FMath::Clamp(ArmorDamage, 0.0f, DamageType->ArmorDamage);

	// Accumulate the resistance damages
	float ResistanceDamage = 0.0f;
	for (const auto Resistance : ResistanceDamages)
	{
		ResistanceDamage += Resistance;
	}

	// Finally we calculate just how much damage should be applied to health
	return Damage + ArmorDamage + ResistanceDamage;
}

void UAtomHealthComponent::TakeDamage(const float Damage, UAtomBaseDamage* DamageType)
{
	// TODO: Maybe consider an event for when something was already dead, just in case so that we can do something with it.
	if (!IsAlive()) return; // Can't do damage to something that is dead.

	const float RealDamage = CalculateDamage(Damage, DamageType);
	Health = FMath::Clamp(Health - RealDamage, 0.0f, MaxHealth);

	OnDamageTaken.Broadcast(RealDamage, DamageType, Health);
	if (Health <= 0.0f)
	{
		OnDeath.Broadcast();
	}
}

void UAtomHealthComponent::Heal(const float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	OnHeal.Broadcast(Amount, Health);
}

bool UAtomHealthComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UAtomHealthComponent::GetHealth() const
{
	return Health;
}
