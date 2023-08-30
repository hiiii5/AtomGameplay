// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AtomInteractableInterface.h"
#include "Interfaces/AtomSaveInterface.h"
#include "AtomInteractable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);

UCLASS(Blueprintable, ClassGroup = (Atom))
class ATOMGAMEPLAY_API AAtomInteractable : public AActor, public IAtomSaveInterface, public IAtomInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomInteractable();

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Usable")
	bool bIsInteractable = true;
	
	UPROPERTY(SaveGame, BlueprintReadOnly, VisibleAnywhere, Category = "Atom|Usable")
	bool bWasUsed;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Usable")
	bool bIsReusable = true;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Usable")
	bool bDestroyOnUse;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Usable")
	FOnInteracted OnInteracted;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool IsInteractable_Implementation() const override;
	virtual void GetInteractableName_Implementation(FString& Name) override;
	virtual bool IsPickup_Implementation() const override;
};
