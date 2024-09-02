// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBall.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterHG.h"



// Sets default values
AInteractableBall::AInteractableBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* StaticMeshComponent;


	// Create and attach a StaticMeshComponent so we can see the ball in the world
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Optionally, set a mesh for the StaticMeshComponent (assuming you have a ball mesh available)
	// StaticMeshComponent->SetStaticMesh(SomeMesh);





}

// Called when the game starts or when spawned
void AInteractableBall::BeginPlay()
{
	Super::BeginPlay();

	// Get the player character
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		// Cast to the specific character class
		ACharacterHG* L1Character = Cast<ACharacterHG>(PlayerCharacter);
		if (L1Character)
		{
			// Set the PlayerRef variable
			PlayerRef = L1Character;
		}
	}
	
}

// Called every frame
void AInteractableBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AInteractableBall::Interact()

{
	// Print a message to the output log
	UE_LOG(LogTemp, Warning, TEXT("Interact called on InteractableBall!"));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact called on InteractableBall"));






}
