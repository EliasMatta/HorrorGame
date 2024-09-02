// DoorInteract.cpp

#include "DoorInteract.h"
#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorInteract::ADoorInteract()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrame->SetupAttachment(RootComponent);

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DoorFrame);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(DoorFrame);
	InteractionSphere->SetSphereRadius(200.0f); // Adjust radius as needed


	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(DoorFrame);
	BoxCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f)); // Adjust size as needed

	bCanInteract = false;
	bRequiresKey = true;
	bIsInteracting = false;
}

// Called when the game starts or when spawned
void ADoorInteract::BeginPlay()
{
	Super::BeginPlay();

	CharacterHG = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoorInteract::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}


	if (!bRequiresKey)
	{
		bCanInteract = true;
	}

	InitialDoorRotation = Door->GetRelativeRotation(); // Store the initial rotation


	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoorInteract::OnBoxBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ADoorInteract::OnBoxEndOverlap);
}

// Called every frame
void ADoorInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);
}


void ADoorInteract::Interact()
{
	// Check if CustomUnlock is enabled
	if (bCustomUnlock)
	{
		// If CustomUnlock is enabled, check if this interaction was explicitly allowed by Blueprint
		if (!bIsInteracting)
		{
			// If bIsInteracting is false, assume it's not called from BP, so block the interaction
			return;
		}
	}
	else
	{
		// If CustomUnlock is false, perform the usual interaction checks
		if (!bCanInteract || bIsInteracting) return; // Check if interaction is allowed and not already interacting
		if (bPaperCheck && !bAllPapersCollected) return; // Check if all papers are collected (if applicable)
		

		bIsInteracting = true; // Set the flag to prevent spamming
	}

	// Continue with door interaction logic
	if (OpenGateSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OpenGateSound, GetActorLocation());
	}

	if (bIsDoorClosed)
	{
		SetDoorOnSameSide(CharacterHG); // Determine door rotation based on player side
		Timeline.Play(); // Play the door opening animation
	}
	else
	{
		Timeline.Reverse(); // Reverse the door opening animation
	}

	bIsDoorClosed = !bIsDoorClosed; // Toggle door state

	// Reset the interaction flag after the timeline ends
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADoorInteract::ResetInteracting, Timeline.GetTimelineLength(), false);
}


void ADoorInteract::ResetInteracting()
{
	bIsInteracting = false;
}

void ADoorInteract::OpenDoor(float Value)
{
	float Angle = bDoorOnSameSide ? DoorRotateAngle : -DoorRotateAngle;

	// Calculate the new rotation based on the initial rotation
	FRotator Rot = InitialDoorRotation + FRotator(0.0f, Angle * Value, 0.0f);

	Door->SetRelativeRotation(Rot);
}

void ADoorInteract::SetDoorOnSameSide(ACharacter* Character)
{
	if (Character)
	{
		FVector CharacterFV = Character->GetActorForwardVector();
		FVector DoorFV = Door->GetForwardVector();
		bDoorOnSameSide = (FVector::DotProduct(CharacterFV, DoorFV) > 0.0f);
	}
}

bool ADoorInteract::TryInteract()
{
	if (IsPlayerInRange())
	{
		bCanInteract = true;
		Interact();
		return true;
	}
	return false;
}

bool ADoorInteract::IsPlayerInRange()
{
	if (CharacterHG)
	{
		return InteractionSphere->IsOverlappingActor(CharacterHG);
	}
	return false;
}

void ADoorInteract::SetRequiresKey(bool RequiresKey)
{
	bRequiresKey = RequiresKey;
	if (!bRequiresKey)
	{
		bCanInteract = true;
	}

}

bool ADoorInteract::GetRequiresKey() const
{
	return bRequiresKey;
}




void ADoorInteract::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != CharacterHG)
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
		if (OverlappingCharacter)
		{
			// Disable player interaction
			bIsInteracting = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Interaction Disabled"));

			// Open the door if it's closed
			if (bIsDoorClosed)
			{
				SetDoorOnSameSide(OverlappingCharacter);
				Interact(); // Open the door
			}
		}
	}
}

void ADoorInteract::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor != CharacterHG)
	{
		ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);


		if (OverlappingCharacter)
		{
		
			bIsInteracting = true;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player Interaction Enabled"));

	
			if (!bIsDoorClosed)
			{
				SetDoorOnSameSide(OverlappingCharacter);
				Interact(); 
			}
		}
	}
}
