// @AngelGuard


#include "Game/AtomButton.h"

#include "Game/AtomSlidingDoor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAtomButton::AAtomButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDestroyOnUse = false;

	ButtonMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMeshComponent"));
	ButtonMeshComponent->SetupAttachment(RootComponent);
	ButtonMeshComponent->SetCollisionProfileName(FName("Custom"));
	ButtonMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Block);
}

// Called when the game starts or when spawned
void AAtomButton::BeginPlay()
{
	Super::BeginPlay();

	if (bStartsActive)
	{
		Interact(nullptr);
	}
}

void AAtomButton::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ButtonMeshComponent->SetStaticMesh(ButtonMesh);
}

// Called every frame
void AAtomButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAtomButton::Load_Implementation(UAtomSaveGame* SaveGame)
{
	if (bWasUsed)
	{
		IAtomInteractableInterface::Execute_Interact(this, nullptr);
	}

	Super::Load_Implementation(SaveGame);
}

void AAtomButton::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);

	// To prevent a cascading interact with doors, we only want to send the interact event when this is not a button, so if this is a player it will activate everything correctly but then send off
	// that the button was the interactor ending the cascade, while still updating the other buttons.
	if (Interactor != nullptr)
	{
		if (Interactor->IsA(AAtomButton::StaticClass()))
		{
			return;
		}
	}

	// Play the sound
	if (ButtonUsedSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonUsedSound, GetActorLocation());
	}

	for (AAtomButton* Button : ButtonsToActivate)
	{
		if (Button == nullptr)
		{
			continue;
		}
		IAtomInteractableInterface::Execute_Interact(Button, this);
	}

	if (Interactor != nullptr)
	{
		if (Interactor->IsA(AAtomSlidingDoor::StaticClass()))
		{
			return;
		}
	}

	// On load, nullptr is passed in, so we need to check for that since the door is being loaded
	if (Interactor == nullptr)
	{
		return;
	}

	for (AAtomInteractable* Interactable : InteractablesToActivate)
	{
		if (Interactable == nullptr)
		{
			continue;
		}
		IAtomInteractableInterface::Execute_Interact(Interactable, this);
	}
}
