// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomInteractable.h"
#include "AtomButton.generated.h"

UCLASS(Blueprintable, ClassGroup = (Atom))
class ATOMGAMEPLAY_API AAtomButton : public AAtomInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomButton();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Atom|Button")
	UStaticMeshComponent* ButtonMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Button")
	UStaticMesh* ButtonMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Button")
	USoundBase* ButtonUsedSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Button")
	TArray<AAtomInteractable*> InteractablesToActivate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Button")
	TArray<AAtomButton*> ButtonsToActivate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Button")
	bool bStartsActive;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Load_Implementation(UAtomSaveGame* SaveGame) override;
	
	virtual void Interact_Implementation(AActor* Interactor) override;
};
