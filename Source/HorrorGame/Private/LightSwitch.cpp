#include "LightSwitch.h"
#include "Engine/PointLight.h"
#include "Kismet/GameplayStatics.h"

ALightSwitch::ALightSwitch()
{
    PrimaryActorTick.bCanEverTick = true;

    SwitchBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchBase"));
    RootComponent = SwitchBase;

    SwitchHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchHandle"));
    SwitchHandle->SetupAttachment(SwitchBase);

    // Initialize initial rotation based on default values
    SwitchHandleInitialRotation = FRotator(0.0f, SwitchRotateAngleOff, 0.0f);
    SwitchHandle->SetRelativeRotation(SwitchHandleInitialRotation, false); // Locks rotation axes

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);
}

void ALightSwitch::BeginPlay()
{
    Super::BeginPlay();

    // Initialize point lights visibility
    for (APointLight* PointLight : PointLights)
    {
        if (PointLight)
        {
            PointLight->GetLightComponent()->SetVisibility(false);
        }
    }

    if (CurveFloat)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindUFunction(this, FName("ToggleSwitch"));
        Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
    }

    if (SoundOnWave && SoundOffWave)
    {
        // Set sound parameters
        AudioComponent->bAutoActivate = false; 
        AudioComponent->SetSound(SoundOffWave);  
    }
}

void ALightSwitch::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Timeline.TickTimeline(DeltaTime);
}

void ALightSwitch::Interact()
{
    if (!Timeline.IsPlaying()) 
    {
        // Toggle switch state
        bIsSwitchOn = !bIsSwitchOn; 

        // Toggle timeline animation
        Timeline.PlayFromStart();

        // Toggle visibility of point lights
        for (APointLight* PointLight : PointLights)
        {
            if (PointLight)
            {
                PointLight->GetLightComponent()->ToggleVisibility();
            }
        }

        // Play sound based on switch state
        if (bIsSwitchOn && SoundOnWave)
        {
            AudioComponent->SetSound(SoundOnWave);
            AudioComponent->SetVolumeMultiplier(SoundOnVolume);
            AudioComponent->SetPitchMultiplier(SoundOnPitch);
            AudioComponent->Play();
        }
        else if (!bIsSwitchOn && SoundOffWave)
        {
            AudioComponent->SetSound(SoundOffWave);
            AudioComponent->SetVolumeMultiplier(SoundOffVolume);
            AudioComponent->SetPitchMultiplier(SoundOffPitch);
            AudioComponent->Play();
        }
    }
}


void ALightSwitch::ToggleSwitch(float Value)
{
    // Determine the target rotation based on switch state
    float TargetAngle = bIsSwitchOn ? SwitchRotateAngleOn : SwitchRotateAngleOff;

    // Calculate the new rotation based on the selected rotation axis
    FRotator CurrentRotation = SwitchHandle->GetRelativeRotation();
    FRotator NewRotation = FRotator::ZeroRotator;

    if (SwitchRotationAxis.X != 0.0f)
    {
        NewRotation.Pitch = TargetAngle;
    }
    else if (SwitchRotationAxis.Y != 0.0f)
    {
        NewRotation.Yaw = TargetAngle;
    }
    else if (SwitchRotationAxis.Z != 0.0f)
    {
        NewRotation.Roll = TargetAngle;
    }

    // Set the relative rotation with locked X, Y, or Z axis
    SwitchHandle->SetRelativeRotation(NewRotation, false);
}
