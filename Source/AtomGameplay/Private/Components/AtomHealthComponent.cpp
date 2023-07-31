#include "Components/AtomHealthComponent.h"
#include "Components/AtomDefenseComponent.h"


// Sets default values for this component's properties
UAtomHealthComponent::UAtomHealthComponent()
{
	// The health component does not perform on tick calculations. These can be performed by helper components.
	PrimaryComponentTick.bCanEverTick = false;
}

void UAtomHealthComponent::TakeDamage(const float Damage, UAtomBaseDamage* DamageType)
{
	// TODO: Maybe consider an event for when something was already dead, just in case so that we can do something with it.
	if (!IsAlive()) return; // Can't do damage to something that is dead.

	// Check for a defense component.
	UAtomDefenseComponent* DefenseComponent = GetOwner()->FindComponentByClass<UAtomDefenseComponent>();
	float RealDamage = 0.0f;
	if(DefenseComponent)
	{
		RealDamage = Damage + DefenseComponent->AccumulateDamages(DamageType->ResistancesDamage);
	}
	else
	{
		// Since there was no defense component, just add all the damages together.
		for (const auto ResistanceDamages : DamageType->ResistancesDamage.Damages)
		{
			RealDamage += ResistanceDamages.Value;
		}
		RealDamage += Damage;
	}
	
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

void UAtomHealthComponent::SetHealth(const float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
}

void UAtomHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAtomHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
