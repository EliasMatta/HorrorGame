	#include "PlayerControllerHG.h"
	#include "EnhancedInputComponent.h"
	#include "EnhancedInputSubsystems.h"
	#include "GameFramework/Character.h"
	#include "GameFramework/CharacterMovementComponent.h"
	#include "Kismet/KismetSystemLibrary.h" 
	#include "GameFramework/Actor.h"
	#include "InterfaceInteract.h"
	#include "BPC_Movement.h"
	#include "Engine/World.h"

	void APlayerControllerHG::BeginPlay()
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}


	}

	void APlayerControllerHG::SetupInputComponent() {

		Super::SetupInputComponent();

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {


			//Jumping
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerControllerHG::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerControllerHG::StopJumping);

			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerControllerHG::Move);


			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerControllerHG::Look);



			EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerControllerHG::HandleInteract);


			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerControllerHG::StartSprinting);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerControllerHG::StopSprinting);

			InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &APlayerControllerHG::HandleMouseButtonDown);
			InputComponent->BindAction("MouseLeftClick", IE_Released, this, &APlayerControllerHG::HandleMouseButtonUp);


			// Example binding for right mouse button
			InputComponent->BindAction("MouseRightClick", IE_Pressed, this, &APlayerControllerHG::OnRightMouseButtonDown);
			InputComponent->BindAction("MouseRightClick", IE_Released, this, &APlayerControllerHG::OnRightMouseButtonUp);

			InputComponent->BindAction("MouseWheel", IE_Pressed, this, &APlayerControllerHG::OnMouseWheelButtonDown);
			InputComponent->BindAction("MouseWheel", IE_Released, this, &APlayerControllerHG::OnMouseWheelButtonUp);


		}

	}


	void APlayerControllerHG::HandleMouseButtonDown()
	{

		bIsMouseButtonDown = true;
		
		if (CurrentNote)
		{
			CurrentNote->OnMouseButtonDown();
		}
	}

	void APlayerControllerHG::HandleMouseButtonUp()
	{
		bIsMouseButtonDown = false;
	
		if (CurrentNote)
		{
			CurrentNote->OnMouseButtonUp();
		}
	}

	void APlayerControllerHG::OnRightMouseButtonDown()
	{
		bIsMouseButtonDown = true;
	}

	void APlayerControllerHG::OnRightMouseButtonUp()
	{
		bIsMouseButtonDown = false;
	}



	void APlayerControllerHG::OnMouseWheelButtonDown()
	{
		bIsMouseButtonDown = true;


	}



	void APlayerControllerHG::OnMouseWheelButtonUp()
	{
		bIsMouseButtonDown = false;
	}

	void APlayerControllerHG::SetCurrentNote(AImmersiveNote* Note)
	{
		if (Note)
		{
			CurrentNote = Note; 
		}
		else
		{
			CurrentNote = nullptr;
		}
	}

	void APlayerControllerHG::StartSprinting()
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
			if (ControlledCharacter)
			{
				UBPC_Movement* MovementComponent = ControlledCharacter->FindComponentByClass<UBPC_Movement>();
				if (MovementComponent && !MovementComponent->bIsSprinting) 
				{
					UCharacterMovementComponent* CharacterMovement = ControlledCharacter->GetCharacterMovement();
					if (CharacterMovement && (CharacterMovement->Velocity.SizeSquared() > 1))
					{
						MovementComponent->StartSprint();
						bIsSprinting = true; 
					}
				}

			}
			
		}
		
	}



	void APlayerControllerHG::StopSprinting()
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			ACharacter* ControlledCharacter = Cast<ACharacter>(ControlledPawn);
			if (ControlledCharacter)
			{
				UBPC_Movement* MovementComponent = ControlledCharacter->FindComponentByClass<UBPC_Movement>();
				if (MovementComponent)
				{
					MovementComponent->StopSprint();
				
				}

			}
		}
		
	}


	void APlayerControllerHG::Jump()
	{



		if (ACharacter* ControlledCharacter = GetCharacter())
		{
			// Start the jump
			ControlledCharacter->Jump();




		}

	}


	void APlayerControllerHG::StopJumping(){

		// Stop the jump
		if (GetCharacter()) {

			GetCharacter()->StopJumping();


		}

	}

	void APlayerControllerHG::Move(const FInputActionValue& Value)
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector2D MovementVector = Value.Get<FVector2D>();


			// Apply movement in the forward direction
			if (MovementVector.Y != 0.f) // Forward/Backward
			{
				//FRotator ControlSpaceRot = GetControlRotation();
				//FVector ForwardDirection = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X);
				FVector ForwardDirection = ControlledPawn->GetActorForwardVector();
				ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
			}



			// Apply movement in the right direction
			if (MovementVector.X != 0.f) // Right/Left
			{

				//FRotator ControlSpaceRot = GetControlRotation();
				//FVector RightDirection = FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y);
				FVector RightDirection = ControlledPawn->GetActorRightVector();
				ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);


			}
		}
	}


	void APlayerControllerHG::AddControllerYawInput(float Val)
	{
		TargetYawInput = bInvertX ? -Val * LookSensitivity : Val * LookSensitivity;
	}

	void APlayerControllerHG::AddControllerPitchInput(float Val)
	{
		TargetPitchInput = bInvertY ? -Val * LookSensitivity : Val * LookSensitivity;
	}

	void APlayerControllerHG::Look(const FInputActionValue& Value)
	{
		  if (bIsMouseButtonDown && CurrentNote)
    {
        // Don't process look input when rotating the note
        return;
    }

		FVector2D LookVector = Value.Get<FVector2D>();

		if (LookVector.X != 0.0f || LookVector.Y != 0.0f)
		{
			bIsMouseActive = true;
			AddControllerYawInput(LookVector.X);
			AddControllerPitchInput(LookVector.Y);
		}
		else
		{
			bIsMouseActive = false;
		}

		APawn* MyPawn = GetPawn();
		if (MyPawn)
		{
			FRotator CurrentRotation = MyPawn->GetControlRotation();

			// Smoothly interpolate the input values towards their target values
			float DeltaTime = GetWorld()->GetDeltaSeconds();
			float InterpolationSpeed = SmoothingSpeed;

			// Apply smoothing to the yaw and pitch inputs
			CurrentYawInput = FMath::FInterpTo(CurrentYawInput, TargetYawInput, DeltaTime, InterpolationSpeed);
			CurrentPitchInput = FMath::FInterpTo(CurrentPitchInput, TargetPitchInput, DeltaTime, InterpolationSpeed);

			// Apply a dead zone to prevent minimal mouse movements from triggering camera updates
			if (FMath::Abs(CurrentYawInput) < DeadZone && FMath::Abs(CurrentPitchInput) < DeadZone)
			{
				CurrentYawInput = 0.0f;
				CurrentPitchInput = 0.0f;
			}

			// Apply camera lag
			float CameraLagSpeed = 15.0f;  // Adjust this value for more/less lag
			FRotator TargetRotation = CurrentRotation;
			TargetRotation.Yaw += CurrentYawInput;
			TargetRotation.Pitch += CurrentPitchInput;

			// Smoothly interpolate between current rotation and target rotation for camera lag
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CameraLagSpeed);

			MyPawn->Controller->SetControlRotation(NewRotation);
		}
	}




	void APlayerControllerHG::HandleInteract() {


		FVector Start = PlayerCameraManager->GetCameraLocation();
		FVector ForwardVector = PlayerCameraManager->GetCameraRotation().Vector();
		FVector End = ((ForwardVector * 350.f) + Start);
		FHitResult HitResult;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		// Perform the line trace
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

		if (bHit && IsValid(HitResult.GetActor()))
		{
			// Check if the hit actor implements the BPI_Interaction interface
			IInterfaceInteract* Interaction = Cast<IInterfaceInteract>(HitResult.GetActor());
			if (Interaction)
			{
				Interaction->Interact();  // Call the interact function on the hit actor

			}
		}
	}
