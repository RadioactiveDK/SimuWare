// Fill out your copyright notice in the Description page of Project Settings.


#include "Arduino.h"

// Sets default values
AArduino::AArduino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArduinoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArduinoMesh"));
	RootComponent = ArduinoMesh;

	// Resize the array to the number of analog pins you want to support
	AnalogPinValues.Init(0.0f, 6);	
	DigitalPinValues.Init(0.0f, 16);

	DigitalPinValues[1] = 0.0f;

	PowerPinValues.Init(0.0f, 7);
}

// Called when the game starts or when spawned
void AArduino::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArduino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

