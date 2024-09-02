// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = 100.0f;
	MinHealth = 0.0f;
	MaxHealth = 100.0f;
	MPC_BloodScreen = nullptr;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::RemoveHealth(float Amount)
{


	CurrentHealth = CurrentHealth -= Amount;
	CurrentHealth = FMath::Clamp(CurrentHealth, MinHealth, MaxHealth);


	UpdateBloodScreen();



}

void UHealthComponent::AddHealth(float Amount)
{


	CurrentHealth = CurrentHealth += Amount;
	CurrentHealth = FMath::Clamp(CurrentHealth, MinHealth, MaxHealth);

	UpdateBloodScreen();



}




void UHealthComponent::UpdateBloodScreen()
{
	if (MPC_BloodScreen == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MPC_BloodScreen is not set."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MPC_BloodScreen is not set."));
		return;
	}

	// Calculate the parameter value based on health level using MapRangeClamped

	float ParameterValue = UKismetMathLibrary::MapRangeClamped(CurrentHealth, MinHealth, MaxHealth, 0.550f, 1.0f);

	// Update the material parameter to control the blood screen effect
	UKismetMaterialLibrary::SetScalarParameterValue(this, MPC_BloodScreen, FName("Amount"), ParameterValue);

	// Log for debugging purposes

	UE_LOG(LogTemp, Log, TEXT("Blood screen updated. ParameterValue: %f"), ParameterValue);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Blood screen updated. ParameterValue: %f"), ParameterValue));
}

