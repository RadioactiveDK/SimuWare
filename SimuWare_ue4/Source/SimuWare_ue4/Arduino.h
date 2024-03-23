// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "GameFramework/Actor.h"
#include "Arduino.generated.h"


UCLASS()
class SIMUWARE_UE4_API AArduino : public APickup
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArduino();

	
	void SetPins();
	void ResetPin1();
	void ResetPin2();
	void ResetPin3();
	FTimerHandle TimerHandle_SetPins;
	FTimerHandle TimerHandle_ResetPin1;
	FTimerHandle TimerHandle_ResetPin2;
	FTimerHandle TimerHandle_ResetPin3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UMeshComponent* ArduinoMesh;

	UPROPERTY(EditAnywhere)
	bool isOn;

	// Array to store analog pin values
	UPROPERTY(EditAnywhere)
	TArray<float> AnalogPinValues;

	UPROPERTY(EditAnywhere)
	TArray<float> DigitalPinValues;

	UPROPERTY(EditAnywhere)
	TArray<float> PowerPinValues;

	void ToggleOnOff();

};
