#include "ImmersiveNote.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerControllerHG.h"
#include "Engine/PostProcessVolume.h"

AImmersiveNote::AImmersiveNote()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsBeingInteracted = false;
    bIsMouseButtonDown = false;
    bHasReachedTarget = false;  // Add a flag to check if the target is reached

    NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteMesh"));
    RootComponent = NoteMesh;

    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(RootComponent);
    InteractionSphere->SetSphereRadius(InteractionRadius);
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);


}

void AImmersiveNote::BeginPlay()
{
    Super::BeginPlay();

    OriginalLocation = GetActorLocation();
    OriginalRotation = GetActorRotation();
}

void AImmersiveNote::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsBeingInteracted)
    {
        if (!bHasReachedTarget)
        {
            MoveTowardsCamera(DeltaTime);
        }
        else
        {
            RotateNoteActor(DeltaTime); 
        }
    }
    else
    {
        ReturnToOriginalPosition(DeltaTime);
    }

   
}

void AImmersiveNote::Interact()
{
    if (!bIsBeingInteracted)
    {
        bIsBeingInteracted = true;
        bHasReachedTarget = false;  // Reset the flag when interaction starts

        APlayerControllerHG* PlayerController = Cast<APlayerControllerHG>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PlayerController)
        {
            FInputModeGameAndUI InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;

            if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
            {
                Character->GetCharacterMovement()->DisableMovement();
            }

            PlayerController->SetIgnoreLookInput(true);
            PlayerController->SetCurrentNote(this);

            if (UserWidgetClass)
            {
                if (CurrentWidget)
                {
                    CurrentWidget->RemoveFromViewport();
                }
                CurrentWidget = CreateWidget<UUserWidget>(PlayerController, UserWidgetClass);
                if (CurrentWidget)
                {
                    CurrentWidget->AddToViewport();
                }
            }
        }

      
    }
    else
    {
        EndInteract();
    }
}

void AImmersiveNote::EndInteract()
{
    bIsBeingInteracted = false;
    bHasReachedTarget = false;  // Reset the flag when interaction ends

    APlayerControllerHG* PlayerController = Cast<APlayerControllerHG>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;

        if (ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn()))
        {
            Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        }

        PlayerController->SetIgnoreLookInput(false);
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromViewport();
            CurrentWidget = nullptr;
        }
    }

 
}

void AImmersiveNote::MoveTowardsCamera(float DeltaTime)
{
    APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (CameraManager)
    {
        FVector CameraLocation = CameraManager->GetCameraLocation();
        FRotator CameraRotation = CameraManager->GetCameraRotation();

        FVector ForwardVector = CameraRotation.Vector();
        FVector TargetLocation = CameraLocation + (ForwardVector * DistanceFromCamera);

        FVector Direction = CameraLocation - GetActorLocation();
        Direction.Normalize();

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            CameraLocation,
            TargetLocation,
            ECC_Visibility,
            QueryParams
        );

        if (bHit)
        {
            TargetLocation = HitResult.ImpactPoint - (ForwardVector * 10.0f);
        }

        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpolationSpeed);
        FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), Direction.Rotation(), DeltaTime, InterpolationSpeed);

        SetActorLocation(NewLocation);
        SetActorRotation(NewRotation);

        // Define tolerance values
        const float LocationTolerance = 1.0f;
        const float RotationTolerance = 1.0f;

        // Check if the actor has reached the target location and rotation within the tolerance
        if (FVector::Dist(NewLocation, TargetLocation) < LocationTolerance &&
            FMath::Abs(FRotator::NormalizeAxis(NewRotation.Yaw - Direction.Rotation().Yaw)) < RotationTolerance &&
            FMath::Abs(FRotator::NormalizeAxis(NewRotation.Pitch - Direction.Rotation().Pitch)) < RotationTolerance)
        {
            bHasReachedTarget = true;
           
        }
    }
}

void AImmersiveNote::ReturnToOriginalPosition(float DeltaTime)
{
    SetActorLocation(FMath::VInterpTo(GetActorLocation(), OriginalLocation, DeltaTime, InterpolationSpeed / 2));
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), OriginalRotation, DeltaTime, InterpolationSpeed / 2));
}

void AImmersiveNote::RotateNoteActor(float DeltaTime)
{
    if (bIsBeingInteracted && bIsMouseButtonDown)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            FVector2D CurrentMousePosition;
            if (PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y))
            {
                FVector2D MouseDelta = CurrentMousePosition - LastMousePosition;

                // Convert rotation speed into radians per second
                float RotationAmount = MouseDelta.X * RotationSpeed * DeltaTime;
               
                // Smooth rotation using FQuat
                FQuat CurrentQuat = GetActorQuat();
                FQuat DeltaQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(RotationAmount));
                FQuat NewQuat = CurrentQuat * DeltaQuat;

                SetActorRotation(NewQuat.Rotator());

                LastMousePosition = CurrentMousePosition;

                
            }
        }
    }
}


void AImmersiveNote::OnMouseButtonDown()
{
    bIsMouseButtonDown = true;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->GetMousePosition(LastMousePosition.X, LastMousePosition.Y);
    }

 
}

void AImmersiveNote::OnMouseButtonUp()
{
    bIsMouseButtonDown = false;

    
}