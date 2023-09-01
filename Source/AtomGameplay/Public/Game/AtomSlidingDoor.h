// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomInteractable.h"
#include "Components/TimelineComponent.h"
#include "AtomSlidingDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorMoved, FVector, LocalPosition);

UCLASS()
class ATOMGAMEPLAY_API AAtomSlidingDoor : public AAtomInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomSlidingDoor();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Atom|Door")
	USceneComponent* DoorRootComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Atom|Door")
	UStaticMeshComponent* DoorMeshComponent;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	UStaticMesh* DoorMesh;
	
	UPROPERTY(SaveGame, BlueprintReadOnly, EditDefaultsOnly, Category="Atom|Door")
	UCurveFloat* DoorCurve;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	USoundBase* DoorUsedSound;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	bool bStartsOpen;
	
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	int32 IsOpen;

	UPROPERTY(SaveGame, BlueprintReadOnly, VisibleAnywhere, Category="Atom|Door")
	bool bIsInMotion;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	FVector OpenedPosition = {0.f, 0.f, 200.f};

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	FVector ClosedPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Atom|Door")
	TArray<AAtomInteractable*> ButtonsToActivate;

	UPROPERTY(BlueprintAssignable, Category="Atom|Door")
	FOnDoorOpened OnDoorOpened;

	UPROPERTY(BlueprintAssignable, Category="Atom|Door")
	FOnDoorClosed OnDoorClosed;

	UPROPERTY(BlueprintAssignable, Category="Atom|Door")
	FOnDoorMoved OnDoorMoved;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	float DoorTimelineValue;
	
	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;
	
	UFUNCTION()
	void TimelineCallback(float Value);

	UFUNCTION()
	void TimelineFinishedCallback() const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Load_Implementation(UAtomSaveGame* SaveGame) override;

	virtual void Interact_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, Category="Atom|Door")
	void ToggleDoor();
	
	UFUNCTION(BlueprintCallable, Category="Atom|Door")
	void ToggleLocked(bool bLocked);

	UFUNCTION(BlueprintCallable, Category="Atom|Door")
	void ForceClose();
};
