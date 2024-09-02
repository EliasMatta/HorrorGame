// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceInteract.h"
#include "Components/TimelineComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"
#include "DoorInteract.generated.h"

UCLASS()
class HORRORGAME_API ADoorInteract : public AActor , public IInterfaceInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact() override;

	ACharacter* CharacterHG;


	 UFUNCTION(BlueprintCallable, Category = "Door Interaction")
	 bool TryInteract();
	 
	UFUNCTION(BlueprintCallable, Category = "Door")
		bool IsPlayerInRange();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
		USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* BoxCollision;

	UFUNCTION(BlueprintCallable, Category = "Door Interaction")
		void SetRequiresKey(bool RequiresKey);

	UFUNCTION(BlueprintPure, Category = "Door Interaction")
		bool GetRequiresKey() const;

	UPROPERTY(EditAnywhere, Category = "Sound") // New sound variable
		USoundBase* OpenGateSound;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FRotator InitialDoorRotation; // New variable to store the initial rotation


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		bool bPaperCheck;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
		bool bAllPapersCollected;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Interaction")
		bool bCustomUnlock;


private:

	UPROPERTY(VisibleAnywhere,Category = "Mesh")
	class UStaticMeshComponent* DoorFrame;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* Door;





protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Interaction")
		bool bRequiresKey;

	FTimeline Timeline;
	UPROPERTY(EditAnywhere)
    UCurveFloat* CurveFloat;

	bool bIsDoorClosed = true;
	bool bCanInteract = false;
	bool bDoorOnSameSide;


	UPROPERTY()
	float DoorRotateAngle = 90.0f;

	UFUNCTION()
	void OpenDoor(float Value);

	
	
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bIsInteracting; 

	void SetDoorOnSameSide(ACharacter* Character);

	void ResetInteracting();




};
