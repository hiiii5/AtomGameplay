#include "../../Public/Components/AtomHealOverTimeComponent.h"
#include "../../Public/Components/AtomHealthComponent.h"


DEFINE_LOG_CATEGORY(LogAtomHealOverTimeComponent);


// Sets default values for this component's properties
UAtomHealOverTimeComponent::UAtomHealOverTimeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAtomHealOverTimeComponent::ApplyHeal()
{
	// If there is no health component then something went wrong in creation. The owner can be set,
	// and a new call to BeginHealOverTime can be made to try again.
	if (!HealthComponent)
	{
		UE_LOG(LogAtomHealOverTimeComponent, Warning, TEXT("UHealOverTimeComponent::ApplyHeal - No health component found on owner of HealOverTimeComponent."));
		return;
	}

	HealthComponent->Heal(HealPerTick);
	ElapsedTime += TickRate;

	if(ElapsedTime >= Duration)
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

		DestroyComponent();
	}
}

bool UAtomHealOverTimeComponent::BeginHealOverTime()
{
	// Get the owners health component
	HealthComponent = GetOwner()->FindComponentByClass<UAtomHealthComponent>();

	if(!HealthComponent)
	{
		return false;
	}

	HealTimerHandle = FTimerHandle();
	GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UAtomHealOverTimeComponent::ApplyHeal, TickRate, true, InitialDelay);

	return true;
}

