// @AngelGuard


#include "Library/AtomInteractableLibrary.h"

#include "Game/AtomInteractable.h"

AAtomInteractable* UAtomInteractableLibrary::SphereCheckForInteractable(UObject* Context, const FVector Start, const FVector Direction, float Distance, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	ensure(Context);

	UWorld* ZaWorldo = Context->GetWorld();
	ensure(ZaWorldo);
	
	TArray<FHitResult> Hits{};
	FVector StartOffset = Start + (Direction.GetSafeNormal() * Radius);
	FVector End = StartOffset + (Direction.GetSafeNormal() * Distance);
	UKismetSystemLibrary::SphereTraceMulti(ZaWorldo, StartOffset, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, Hits, true, TraceColor, TraceHitColor, DrawTime);

	if (Hits.Num() <= 0)
	{
		return nullptr;
	}

	AAtomInteractable* InteractableActor = nullptr;

	float ClosestInteractableDistance = Distance;
	for (FHitResult& Hit : Hits)
	{
		if (!Hit.bBlockingHit)
		{
			continue;
		}

		if (!Hit.GetActor())
		{
			continue;	
		}

		if (!Hit.GetActor()->IsA(AAtomInteractable::StaticClass()))
		{
			continue;
		}

		if (Hit.Distance < ClosestInteractableDistance)
		{
			ClosestInteractableDistance = Hit.Distance;
			if (Hit.GetActor() != InteractableActor)
			{
				InteractableActor = Cast<AAtomInteractable>(Hit.GetActor());
			}
		}
	}

	return InteractableActor;
}
