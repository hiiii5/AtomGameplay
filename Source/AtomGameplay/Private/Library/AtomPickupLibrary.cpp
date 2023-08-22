// @AngelGuard


#include "../../Public/Library/AtomPickupLibrary.h"
#include "Game/AtomPickup.h"

TArray<AAtomPickup*> UAtomPickupLibrary::CheckForPickupsInRadius(UObject* Context, AActor* Actor, float Radius)
{
	// Ensure the actor and the context is valid
	ensure(Actor);
	ensure(Context);

	// Get the world
	UWorld* World = Context->GetWorld();
	ensure(World);

	// Perform a sphere trace for pickups around the actor
	TArray<FHitResult> HitResults{};
	FCollisionQueryParams QueryParams{};
	QueryParams.AddIgnoredActor(Actor);
	World->SweepMultiByChannel(HitResults, Actor->GetActorLocation(), Actor->GetActorLocation(), FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(Radius), QueryParams);

	TArray<AAtomPickup*> Pickups{};
	// Iterate through the hit results and add the pickups to the array
	for (auto HitResult : HitResults)
	{
		AAtomPickup* Pickup = Cast<AAtomPickup>(HitResult.GetActor());
		if (Pickup)
		{
			Pickups.Add(Pickup);
		}
	}

	// Sort the pickups by distance from least to greatest
	Pickups.Sort([Actor](const AAtomPickup& A, const AAtomPickup& B)
	{
		return FVector::Distance(A.GetActorLocation(), Actor->GetActorLocation()) < FVector::Distance(B.GetActorLocation(), Actor->GetActorLocation());
	});

	return Pickups;
}

AAtomPickup* UAtomPickupLibrary::GetClosestPickup(UObject* Context, AActor* Actor, const float Radius)
{
	ensure(Actor);
	ensure(Context);

	TArray<AAtomPickup*> Pickups = CheckForPickupsInRadius(Context, Actor, Radius);
	if (Pickups.Num() <= 0)
	{
		return nullptr;
	}

	return Pickups[0];
}

bool UAtomPickupLibrary::InteractWithClosestPickup(UObject* Context, AActor* Actor, const float Radius)
{
	ensure(Actor);
	ensure(Context);

	TArray<AAtomPickup*> Pickups = CheckForPickupsInRadius(Context, Actor, Radius);
	if (Pickups.Num() <= 0)
	{
		return false;
	}

	// Interact with the closest pickup
	Pickups[0]->Interact(Actor);
	return true;
}
