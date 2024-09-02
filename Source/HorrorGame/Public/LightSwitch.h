#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceInteract.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/PointLightComponent.h"
#include "Sound/SoundWave.h" // Include SoundWave header
#include "Components/AudioComponent.h" // Include AudioComponent header
#include "LightSwitch.generated.h"

UCLASS()
class HORRORGAME_API ALightSwitch : public AActor, public IInterfaceInteract
{
    GENERATED_BODY()

public:
    ALightSwitch();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void Interact() override;

    UPROPERTY(EditAnywhere, Category = "Lights")
        TArray<class APointLight*> PointLights;

    UPROPERTY(EditAnywhere, Category = "Switch Settings")
        FVector SwitchRotationAxis = FVector(0.0f, 0.0f, 1.0f); // Default to rotating around the Z axis

    UPROPERTY(EditAnywhere, Category = "Switch Settings")
        float SwitchRotateAngleOn = -90.0f; // Angle for switch on position

    UPROPERTY(EditAnywhere, Category = "Switch Settings")
        float SwitchRotateAngleOff = 0.0f; // Angle for switch off position

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundWave* SoundOnWave;

    UPROPERTY(EditAnywhere, Category = "Audio")
        float SoundOnVolume = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Audio")
        float SoundOnPitch = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Audio")
        USoundWave* SoundOffWave;

    UPROPERTY(EditAnywhere, Category = "Audio")
        float SoundOffVolume = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Audio")
        float SoundOffPitch = 1.0f;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* SwitchBase;

    UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* SwitchHandle;

    UPROPERTY(EditAnywhere, Category = "Timeline")
        UCurveFloat* CurveFloat;

    FTimeline Timeline;

    bool bIsSwitchOn = false;

    FRotator SwitchHandleInitialRotation;

    UPROPERTY()
        UAudioComponent* AudioComponent;

    UFUNCTION()
        void ToggleSwitch(float Value);

 
};
