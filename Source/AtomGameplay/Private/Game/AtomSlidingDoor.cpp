// @AngelGuard


#include "../../Public/Game/AtomSlidingDoor.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AAtomSlidingDoor::AAtomSlidingDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the root
	DoorRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DoorRootComponent"));
	RootComponent = DoorRootComponent;

	// Construct the door mesh
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMeshComponent"));
	DoorMeshComponent->SetupAttachment(DoorRootComponent);
	DoorMeshComponent->SetCollisionProfileName(FName("Custom"));
	DoorMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Block);
}

// Called when the game starts or when spawned
void AAtomSlidingDoor::BeginPlay()
{
	// Create callback events.
	FOnTimelineFloat OnTimelineFloat;
	FOnTimelineEventStatic OnTimelineFinished;
	
	Super::BeginPlay();

	if (DoorCurve)
	{
		// Create the door timeline
		DoorTimeline = NewObject<UTimelineComponent>(this, FName("DoorTimeline"));
		DoorTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		// Add the timeline to this actor's blueprint
		this->BlueprintCreatedComponents.Add(DoorTimeline);
		// Set the name to a static name so that this timeline replicates.
		DoorTimeline->SetNetAddressable();

		// Set the timeline direction variable
		DoorTimeline->SetPropertySetObject(this);
		DoorTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

		// Set if the timeline should loop
		DoorTimeline->SetLooping(false);
		// Set the timeline length to the last keyframe of the curve.
		DoorTimeline->SetTimelineLength(DoorCurve->FloatCurve.GetLastKey().Time);
		// Set the timeline length mode to the last keyframe of the curve.
		DoorTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		// Set the playback position to the start or end of the timeline depending on if the door starts open or not.
		if (bStartsOpen)
		{
			DoorTimeline->SetPlaybackPosition(1.f, false);
		}
		else
		{
			DoorTimeline->SetPlaybackPosition(0.f, false);
		}

		// Bind the update timeline function and the finished timeline function
		OnTimelineFloat.BindUFunction(this, FName("TimelineCallback"));
		OnTimelineFinished.BindUFunction(this, FName("TimelineFinishedCallback"));

		// Bind the door curve to the timeline
		DoorTimeline->AddInterpFloat(DoorCurve, OnTimelineFloat);
		// Bind the timeline finished function to the timeline
		DoorTimeline->SetTimelineFinishedFunc(OnTimelineFinished);

		// Register the timeline component with this actor.
		DoorTimeline->RegisterComponent();
	}
}

void AAtomSlidingDoor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bStartsOpen)
	{
		bIsOpen = true;
		DoorTimelineValue = 1.f;
		DoorMeshComponent->SetRelativeLocation(OpenedPosition);
	}
	else
	{
		bIsOpen = false;
		DoorTimelineValue = 0.f;
		DoorMeshComponent->SetRelativeLocation(ClosedPosition);
	}

	DoorMeshComponent->SetStaticMesh(DoorMesh);
}

void AAtomSlidingDoor::TimelineCallback(float Value)
{
	DoorTimelineValue = Value;

	// Interp the doors position based on the doors timeline value
	const FVector NewLocation = FMath::Lerp(ClosedPosition, OpenedPosition, DoorTimelineValue);
	DoorMeshComponent->SetRelativeLocation(NewLocation);
	OnDoorMoved.Broadcast(NewLocation);
}

void AAtomSlidingDoor::TimelineFinishedCallback() const
{
	(bIsOpen) ? OnDoorOpened.Broadcast() : OnDoorClosed.Broadcast();
}

// Called every frame
void AAtomSlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorTimeline)
	{
		DoorTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

void AAtomSlidingDoor::Load_Implementation(UAtomSaveGame* SaveGame)
{
	if(bIsOpen)
	{
		DoorTimelineValue = 1.f;
		DoorMeshComponent->SetRelativeLocation(OpenedPosition);
	}
	else
	{
		DoorTimelineValue = 0.f;
		DoorMeshComponent->SetRelativeLocation(ClosedPosition);
	}
	
	Super::Load_Implementation(SaveGame);
}

void AAtomSlidingDoor::Interact_Implementation(AActor* Interactor)
{
	ToggleDoor();

	for (AAtomInteractable* Interactable : ButtonsToActivate)
	{
		if (Interactable == nullptr)
		{
			continue;
		}
		IAtomInteractableInterface::Execute_Interact(Interactable, this);
	}
}

void AAtomSlidingDoor::ToggleDoor()
{
	if (!DoorTimeline)
	{
		UE_LOG(LogActor, Warning, TEXT("AAtomDoor::ToggleDoor() - DoorTimeline is null"));
		return;
	}

	if (!bIsInteractable)
	{
		return;
	}

	if (DoorUsedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorUsedSound, GetActorLocation());
	}
	
	// To allow the door to be interacted with while it is in motion
	if (bIsOpen || (bIsOpen && bIsInMotion))
	{
		DoorTimeline->Reverse();
		bIsOpen = false;
		bWasUsed = false;
	}
	else if (!bIsOpen || (!bIsOpen && bIsInMotion))
	{
		DoorTimeline->Play();
		bIsOpen = true;
		bWasUsed = true;
	}
}

void AAtomSlidingDoor::ToggleLocked(const bool bLocked)
{
	bIsInteractable = !bLocked;
}

void AAtomSlidingDoor::ForceClose()
{
	DoorTimeline->Reverse();
	bIsOpen = false;
	bWasUsed = false;
}

