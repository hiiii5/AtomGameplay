// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomInteractable.h"
#include "AtomPickup.generated.h"

class USphereComponent;
class UAtomPickupData;

USTRUCT(BlueprintType)
struct FAtomPickupInfo
{
	GENERATED_BODY()

	FAtomPickupInfo()
		: PickupData(nullptr),
		  Quantity(1)
	{
	}
	
	FAtomPickupInfo(const int32 Quantity)
		: PickupData(nullptr),
		  Quantity(Quantity)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	UAtomPickupData* PickupData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	int32 Quantity = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickedUp, FAtomPickupInfo, PickupInfo, AActor*, Interactor);

UCLASS(Blueprintable, ClassGroup = "Atom")
class ATOMGAMEPLAY_API AAtomPickup : public AAtomInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomPickup();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Atom|Pickup")
	USphereComponent* PickupSphere;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Pickup")
	FAtomPickupInfo PickupInfo;

	UPROPERTY(BlueprintAssignable, Category = "Atom|Pickup")
	FOnPickedUp OnPickedUp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual void GetInteractableName_Implementation(FString& Name) override;
	virtual bool IsPickup_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Atom|Pickup")
	void Pickup(AActor* Interactor);
};
