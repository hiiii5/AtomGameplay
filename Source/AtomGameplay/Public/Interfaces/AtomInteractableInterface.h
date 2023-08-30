// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AtomInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAtomInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATOMGAMEPLAY_API IAtomInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Interactable")
	void Interact(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Interactable")
	bool IsInteractable() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Interactable")
	void Highlight(bool bHighlight);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Interactable")
	void GetInteractableName(FString &Name);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Atom|Interactable")
	bool IsPickup() const;
};
