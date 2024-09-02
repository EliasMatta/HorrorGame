#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceInteract.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "ImmersiveNote.generated.h"

UCLASS()
class HORRORGAME_API AImmersiveNote : public AActor, public IInterfaceInteract
{
    GENERATED_BODY()

public:
    AImmersiveNote();

    virtual void Tick(float DeltaTime) override;
    virtual void Interact() override;

    UFUNCTION(BlueprintCallable, Category = "Immersive Note")
    void EndInteract();

    void OnMouseButtonDown();
    void OnMouseButtonUp();
    void RotateNoteActor(float DeltaTime);

    bool bIsRotated; // Add this line
    bool bIsRotating;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* NoteMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* InteractionSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    float InterpolationSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    float DistanceFromCamera = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    float RotationSpeed = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    float InteractionRadius = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    TSubclassOf<UUserWidget> UserWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Note")
    TSubclassOf<AActor> NoteExaminationClass;

private:
    void MoveTowardsCamera(float DeltaTime);
    void ReturnToOriginalPosition(float DeltaTime);

    bool bIsBeingInteracted;
    bool bHasReachedTarget;

    FVector OriginalLocation;
    FRotator OriginalRotation;
    UUserWidget* CurrentWidget;

    bool bIsMouseButtonDown;
    FVector2D LastMousePosition;
    TArray<AActor*> FoundNotes;
};
