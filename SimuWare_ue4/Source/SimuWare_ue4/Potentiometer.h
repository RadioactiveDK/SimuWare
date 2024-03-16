// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArduinoComponent.h"
#include "Potentiometer.generated.h"

UCLASS()
class SIMUWARE_UE4_API APotentiometer : public AArduinoComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotentiometer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PotentiometerMesh;

	UPROPERTY(EditAnywhere)
	float AmplificationFactor = 1.0f;
};
