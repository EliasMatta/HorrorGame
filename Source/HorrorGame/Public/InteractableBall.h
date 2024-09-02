// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterfaceInteract.h"
#include "InteractableBall.generated.h"

UCLASS()
class HORRORGAME_API AInteractableBall : public AActor , public IInterfaceInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Interact() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player")
		class ACharacterHG* PlayerRef;



};
