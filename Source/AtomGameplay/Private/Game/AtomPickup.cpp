// @AngelGuard


#include "Game/AtomPickup.h"

#include "Components/SphereComponent.h"
#include "Data/AtomPickupData.h"

// Sets default values
AAtomPickup::AAtomPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick= true;

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetCollisionProfileName("OverlapAll");
	PickupSphere->SetGenerateOverlapEvents(true);
	PickupSphere->SetSphereRadius(100.0f);
	
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

