// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ImmersiveNote.h"
#include "BPC_Movement.h" // Include your BPC_Movement header
#include "PlayerControllerHG.generated.h"

/**
 *
 */

UCLASS()
class HORRORGAME_API APlayerControllerHG : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	AImmersiveNote* CurrentNote;

	void SetCurrentNote(AImmersiveNote* Note);

	void HandleMouseButtonDown();
	void HandleMouseButtonUp();


	void OnRightMouseButtonDown();
	void OnRightMouseButtonUp();

	void OnMouseWheelButtonDown();
	void OnMouseWheelButtonUp();	


	virtual void SetupInputComponent() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), Category = "Input")
		class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), Category = "Input")
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), Category = "Input")
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true") , Category = "Input")
		class UInputAction* LookAction;

	// Input action for interacting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), Category = "Input")
		class UInputAction* IA_Interact;


	// Input action for sprinting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), Category = "Input")
		class UInputAction* SprintAction;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "0.01", ClampMax = "10.0", UIMin = "0.01", UIMax = "10.0"))
	float LookSensitivity = 1.0f;




	// Smoothing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (ClampMin = "0.01", ClampMax = "10.0", UIMin = "0.01", UIMax = "10.0"))
		float SmoothingSpeed = 6.0f;

	// Invert controls
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		bool bInvertX = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		bool bInvertY = false;

	// Dead zone to prevent minimal mouse movements from triggering camera updates
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
		float DeadZone = 0.05f; // Adjust the dead zone value as needed


	UPROPERTY(BlueprintReadOnly, Category = "Sprint")
		bool bIsSprinting = false;

	// Multiplier for sprint speed relative to the character's default movement speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint", meta = (ClampMin = "1.0", UIMin = "1.0"))
		float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float RotationSpeed = 1.0f;


	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
		bool bIsInteractingWithNote;


	void StartSprinting();
	void StopSprinting();


	float TargetYawInput = 0.0f;
	float TargetPitchInput = 0.0f;

	float CurrentYawInput = 0.0f;
	float CurrentPitchInput = 0.0f;

	bool bIsMouseButtonDown;
	bool bIsMouseActive = false;
	bool bIsRotatingNote = false;
	FVector2D LastMousePosition;


	UFUNCTION()
		void Jump();

	UFUNCTION()
		void StopJumping();

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	virtual void AddControllerYawInput(float Val) ;
	virtual void AddControllerPitchInput(float Val);


	UFUNCTION()
	void Look(const FInputActionValue& Value);

	
	void HandleInteract();

	UPROPERTY()
	UBPC_Movement* BPCMovementComponent; // Declare the BPC_Movement component

};