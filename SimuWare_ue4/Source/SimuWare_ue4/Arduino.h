// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arduino.generated.h"

UCLASS()
class SIMUWARE_UE4_API AArduino : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArduino();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UMeshComponent* ArduinoMesh;

	// Array to store analog pin values
	UPROPERTY(EditAnywhere)
	TArray<float> AnalogPinValues;

	UPROPERTY(EditAnywhere)
	TArray<float> DigitalPinValues;

	UPROPERTY(EditAnywhere)
	TArray<float> PowerPinValues;

};
