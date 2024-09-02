#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SpotLightComponent.h"
#include "FlashlightComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORGAME_API UFlashlightComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFlashlightComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void ToggleFlashlight();

    UFUNCTION(BlueprintCallable)
        void Initialize(AActor* Player);


	UFUNCTION(BlueprintCallable)
    void AddBatteryLife(float Amount);


    void DepleteBatteryLife();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        USpotLightComponent* FlashlightLight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float MaxBatteryLevel = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float CurrentBatteryLevel = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float MinBatteryLevel = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float DepletionAmount = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float DepletionSpeed = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float MaxIntensity = 3000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flashlight")
        float MinIntensity = 500.0f;

private:
    AActor* Player;
    FTimerHandle DepletionTimerHandle;

    void UpdateLightIntensity();
};
