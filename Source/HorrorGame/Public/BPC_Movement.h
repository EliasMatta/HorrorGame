#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BPC_Movement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSprintTimerDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HORRORGAME_API UBPC_Movement : public UActorComponent
{
    GENERATED_BODY()

public:
    UBPC_Movement();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable)
        void Initialize();

    UFUNCTION(BlueprintCallable)
        void StartSprint();

    UFUNCTION(BlueprintCallable)
        void StopSprint();

    UFUNCTION(BlueprintCallable)
        void RestoreStamina();

    UPROPERTY(BlueprintAssignable)
        FSprintTimerDelegate SprintTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
        float SprintSpeed = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
        float WalkSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
        float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
        float CurrentStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
        float MinStamina = 0.0f;

    UPROPERTY(BlueprintReadOnly)
        bool bIsSprinting = false; 

    float StaminaRestoreRate = 2.0f;

private:
    FTimerHandle SprintTimerHandle;

    void SetPlayerMaxSpeed(float MaxSpeed);
    void OnSprintTimerComplete();
};
