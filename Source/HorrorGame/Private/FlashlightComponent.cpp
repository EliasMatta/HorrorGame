#include "FlashlightComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UFlashlightComponent::UFlashlightComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    FlashlightLight = nullptr;
    Player = nullptr;
}

void UFlashlightComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update light intensity based on current battery level
    UpdateLightIntensity();
}

void UFlashlightComponent::ToggleFlashlight()
{
    if (FlashlightLight)
    {
        bool bIsVisible = FlashlightLight->IsVisible();

        if (CurrentBatteryLevel > MinBatteryLevel)
        {
            bIsVisible = !bIsVisible;

            FlashlightLight->SetVisibility(bIsVisible, true);

            if (bIsVisible)
            {
                GetWorld()->GetTimerManager().SetTimer(
                    DepletionTimerHandle,
                    this,
                    &UFlashlightComponent::DepleteBatteryLife,
                    DepletionSpeed,
                    true
                );
            }
            else
            {
                GetWorld()->GetTimerManager().ClearTimer(DepletionTimerHandle);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FlashlightComponent: Battery is too low to toggle flashlight."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FlashlightComponent: FlashlightLight is not set."));
    }
}

void UFlashlightComponent::Initialize(AActor* InPlayer)
{
    Player = InPlayer;

    if (Player)
    {
        FlashlightLight = Player->FindComponentByClass<USpotLightComponent>();

        if (!FlashlightLight)
        {
            UE_LOG(LogTemp, Warning, TEXT("FlashlightComponent: No SpotLightComponent found on Player."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FlashlightComponent: Player is not set."));
    }
}

void UFlashlightComponent::AddBatteryLife(float Amount)
{
    CurrentBatteryLevel += Amount;
    CurrentBatteryLevel = FMath::Clamp(CurrentBatteryLevel, MinBatteryLevel, MaxBatteryLevel);
}

void UFlashlightComponent::DepleteBatteryLife()
{
    CurrentBatteryLevel -= DepletionAmount;
    CurrentBatteryLevel = FMath::Clamp(CurrentBatteryLevel, MinBatteryLevel, MaxBatteryLevel);

    if (CurrentBatteryLevel <= MinBatteryLevel)
    {
        FlashlightLight->SetVisibility(false, true);
        GetWorld()->GetTimerManager().ClearTimer(DepletionTimerHandle);
    }
}

void UFlashlightComponent::UpdateLightIntensity()
{
    if (FlashlightLight)
    {
        // Interpolate intensity based on battery level
        float IntensityRange = MaxIntensity - MinIntensity;
        float BatteryPercentage = (CurrentBatteryLevel - MinBatteryLevel) / (MaxBatteryLevel - MinBatteryLevel);
        float CurrentIntensity = FMath::Lerp(MinIntensity, MaxIntensity, BatteryPercentage);

        FlashlightLight->SetIntensity(CurrentIntensity);
    }
}
