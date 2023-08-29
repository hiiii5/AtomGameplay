// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AtomInteractableLibrary.generated.h"

class AAtomInteractable;
/**
 * 
 */
UCLASS()
class ATOMGAMEPLAY_API UAtomInteractableLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Atom|Interactable", meta = (WorldContext = "Context", AutoCreateRefTerm = "ActorsToIgnore", AdvancedDisplay = "TraceColor,TraceHitColor,DrawTime"))
	static AAtomInteractable* SphereCheckForInteractable(UObject* Context, const FVector Start, const FVector Direction, float Distance, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = 5.0f);
};
