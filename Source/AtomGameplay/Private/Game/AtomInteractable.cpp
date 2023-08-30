// @AngelGuard


#include "../../Public/Game/AtomInteractable.h"


// Sets default values
AAtomInteractable::AAtomInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAtomInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAtomInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAtomInteractable::Interact_Implementation(AActor* Interactor)
{
	if (!bIsInteractable)
	{
		return;
	}
	
	if (bDestroyOnUse)
	{
		OnInteracted.Broadcast(Interactor);
		this->Destroy();
		return;
	}

	if (bIsReusable)
	{
		bWasUsed = !bWasUsed;
		OnInteracted.Broadcast(Interactor);
		return;
	}

	if (!bWasUsed)
	{
		bWasUsed = true;
		OnInteracted.Broadcast(Interactor);
	}
}

bool AAtomInteractable::IsInteractable_Implementation() const
{
	return bIsInteractable;
}

void AAtomInteractable::GetInteractableName_Implementation(FString& Name)
{
	Name = this->GetName();
}

bool AAtomInteractable::IsPickup_Implementation() const
{
	return false;
}

