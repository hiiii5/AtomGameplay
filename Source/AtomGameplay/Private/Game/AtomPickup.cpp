// @AngelGuard


#include "Game/AtomPickup.h"
#include "Data/AtomPickupData.h"

// Sets default values
AAtomPickup::AAtomPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pickups generally will be removed from the world on pickup, like an inventory item.
	bDestroyOnUse = true;
	bIsReusable = false;
}

// Called when the game starts or when spawned
void AAtomPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAtomPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// As a convenience we want to set the pickup's info pickup data to the pickup data asset's info.
	// This way we can easily access the pickup data from the pickup info.
	// We really don't want these things to be out of sync.
	PickupInfo.PickupData = PickupData;
}

// Called every frame
void AAtomPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAtomPickup::Interact_Implementation(AActor* Interactor)
{
	OnPickedUp.Broadcast(PickupInfo, Interactor);
	Super::Interact_Implementation(Interactor);
}

void AAtomPickup::Pickup(AActor* Interactor)
{
	IAtomInteractableInterface::Execute_Interact(this, Interactor);
}

