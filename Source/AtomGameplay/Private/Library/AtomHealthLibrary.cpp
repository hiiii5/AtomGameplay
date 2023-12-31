﻿#include "Library/AtomHealthLibrary.h"
#include "Components/AtomDamageOverTimeComponent.h"
#include "Components/AtomHealOverTimeComponent.h"
#include "Components/AtomHealthComponent.h"


void UAtomHealthLibrary::TakeDamage(AActor* Actor, const float Damage, const FDamageStats DamageStats)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return;
	}

	UAtomHealthComponent* HealthComponent = Actor->FindComponentByClass<UAtomHealthComponent>();
	if(!HealthComponent)
	{
		return;
	}
	
	UAtomBaseDamage* DamageType = CreateDamageType(DamageStats);
	HealthComponent->TakeDamage(Damage, DamageType);
}

void UAtomHealthLibrary::Heal(AActor* Actor, const float Amount)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return;
	}

	// Now we need to make sure that the actor actually has the health component.
	UAtomHealthComponent* HealthComponent = Actor->FindComponentByClass<UAtomHealthComponent>();
	if(!HealthComponent)
	{
		return;
	}

	HealthComponent->Heal(Amount);
}

bool UAtomHealthLibrary::HealOverTime(AActor* Actor, const float HealPerTick, const float Duration, const float TickRate, const float InitialDelay)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return false;
	}

	// Add the heal over time component to the actor.
	UAtomHealOverTimeComponent* Comp = Cast<UAtomHealOverTimeComponent>(Actor->AddComponentByClass(UAtomHealOverTimeComponent::StaticClass(), false, Actor->GetTransform(), false));
	Comp->Duration = Duration;
	Comp->TickRate = TickRate;
	Comp->InitialDelay = InitialDelay;
	Comp->HealPerTick = HealPerTick;
	
	return Comp->BeginHealOverTime();
}

bool UAtomHealthLibrary::DamageOverTime(AActor* Actor, float DamagePerTick, float Duration, float TickRate, float InitialDelay)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return false;
	}

	// Add the damage over time component to the actor
	UAtomDamageOverTimeComponent* Comp = Cast<UAtomDamageOverTimeComponent>(Actor->AddComponentByClass(UAtomDamageOverTimeComponent::StaticClass(), false, Actor->GetTransform(), false));
	Comp->Duration = Duration;
	Comp->TickRate = TickRate;
	Comp->InitialDelay = InitialDelay;
	Comp->DamagePerTick = DamagePerTick;

	return Comp->BeginDamageOverTime();
}

bool UAtomHealthLibrary::IsAlive(AActor* Actor)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return false;
	}

	// Now we need to make sure that the actor actually has the health component.
	const UAtomHealthComponent* HealthComponent = Actor->FindComponentByClass<UAtomHealthComponent>();
	if(!HealthComponent)
	{
		return false;
	}

	return HealthComponent->IsAlive();
}

float UAtomHealthLibrary::GetHealth(AActor* Actor)
{
	// To prevent a crash, check if the actor is valid.
	if (!Actor)
	{
		return 0.0f;
	}

	// Now we need to make sure that the actor actually has the health component.
	const UAtomHealthComponent* HealthComponent = Actor->FindComponentByClass<UAtomHealthComponent>();
	if(!HealthComponent)
	{
		return 0.0f;
	}

	return HealthComponent->GetHealth();
}

float UAtomHealthLibrary::GetMaxHealth(AActor *Actor)
{
    if (!IsValid(Actor))
	{
		return 0.0f;
	}

	const UAtomHealthComponent* HealthComponent = Actor->FindComponentByClass<UAtomHealthComponent>();
	if (!IsValid(HealthComponent))
	{
		return 0.0f;
	}
	
	return HealthComponent->GetMaxHealth();
}

UAtomBaseDamage* UAtomHealthLibrary::CreateDamageType(const FDamageStats DamageStats)
{
	UAtomBaseDamage* DamageType = NewObject<UAtomBaseDamage>();
	DamageType->ResistancesDamage = DamageStats;
	return DamageType;
}
