#include "../../Public/Components/AtomDamageOverTimeComponent.h"
#include "../../Public/Components/AtomHealthComponent.h"


DEFINE_LOG_CATEGORY(LogAtomDamageOverTimeComponent);


// Sets default values for this component's properties
UAtomDamageOverTimeComponent::UAtomDamageOverTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAtomDamageOverTimeComponent::ApplyDamage()
{
	// If there is no health component then something went wrong in creation. The owner can be set,
	// and a new call to BeginDamageOverTime can be made to try again.
	if (!HealthComponent)
	{
		UE_LOG(LogAtomDamageOverTimeComponent, Warning, TEXT("UDamageOverTimeComponent::ApplyDamage - No health component found on owner of DamageOverTimeComponent."));
		return;
	}

	if(!DamageType)
	{
		UE_LOG(LogAtomDamageOverTimeComponent, Warning, TEXT("UDamageOverTimeComponent::ApplyDamage - No damage type set on DamageOverTimeComponent."));
		return;
	}

	// Apply the damage to the health component
	HealthComponent->TakeDamage(DamagePerTick, DamageType);
	ElapsedTime += TickRate;

	if(ElapsedTime >= Duration)
	{
		// TODO: Is this really correct? What if the player moves worlds, or the world isn't spawned yet? Is that even possible?
		GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
		
		// remove and destroy this component
		DestroyComponent();
	}
}

bool UAtomDamageOverTimeComponent::BeginDamageOverTime()
{
	// Get the owners health component
	HealthComponent = GetOwner()->FindComponentByClass<UAtomHealthComponent>();

	// If there was no health component found then return false
	if (!HealthComponent)
	{
		return false;
	}
	
	DamageTimerHandle = FTimerHandle();
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &UAtomDamageOverTimeComponent::ApplyDamage, TickRate, true, InitialDelay);

	return true;
}
