#include "BPC_Movement.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UBPC_Movement::UBPC_Movement()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UBPC_Movement::BeginPlay()
{
    Super::BeginPlay();
    Initialize();
}

void UBPC_Movement::Initialize()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character)
    {
        UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = WalkSpeed;
         
        }
    }
   
}

void UBPC_Movement::StartSprint()
{
    if (CurrentStamina > MinStamina)
    {
        SetPlayerMaxSpeed(SprintSpeed);

        // Start a timer for sprinting
        GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &UBPC_Movement::OnSprintTimerComplete, 0.1f, true);

        // Broadcast the SprintTimer event
        SprintTimer.Broadcast();

        bIsSprinting = true; // Set sprinting flag
    }
  
}

void UBPC_Movement::StopSprint()
{
    GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
    SetPlayerMaxSpeed(WalkSpeed);
    bIsSprinting = false; // Reset sprinting flag

 
}

void UBPC_Movement::SetPlayerMaxSpeed(float MaxSpeed)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character)
    {
        UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
        if (CharacterMovement)
        {
            CharacterMovement->MaxWalkSpeed = MaxSpeed;

       
        }
    }
}

void UBPC_Movement::OnSprintTimerComplete()
{
    CurrentStamina -= 1.0f;

    CurrentStamina = FMath::Clamp(CurrentStamina, MinStamina, MaxStamina);

    if (CurrentStamina <= MinStamina)
    {
        StopSprint();
    }


}

void UBPC_Movement::RestoreStamina()
{
    // Calculate the amount of stamina to restore based on time since last tick
    float DeltaTime = GetWorld()->GetDeltaSeconds();
    float StaminaToAdd = StaminaRestoreRate * DeltaTime;

    // Update the current stamina
    CurrentStamina += StaminaToAdd;

    // Clamp the stamina within the allowed range
    CurrentStamina = FMath::Clamp(CurrentStamina, MinStamina, MaxStamina);

 
}


void UBPC_Movement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Restore stamina over time if not sprinting
    if (CurrentStamina < MaxStamina && !bIsSprinting)
    {
        RestoreStamina();
    }
}
