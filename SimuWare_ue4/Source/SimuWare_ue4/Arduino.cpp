// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "Engine/World.h"
#include "Arduino.h"

// Sets default values
AArduino::AArduino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Resize the array to the number of analog pins you want to support
	AnalogPinValues.Init(0.0f, 6);	
	DigitalPinValues.Init(0.0f, 16);
	PowerPinValues.Init(0.0f, 7);
}

// Called when the game starts or when spawned
void AArduino::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SetPins, this, &AArduino::SetPins, 3.0f,true,0.0f);
	
}

// Called every frame
void AArduino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArduino::SetPins()
{

    DigitalPinValues[1] = 3.0f;
    GetWorldTimerManager().SetTimer(TimerHandle_ResetPin1, this, &AArduino::ResetPin1,1.0f, false);

}

void AArduino::ResetPin1()
{
    DigitalPinValues[1] = 0.0f;
	DigitalPinValues[3] = 3.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_ResetPin3, this, &AArduino::ResetPin3,1.0f, false);
}
void AArduino::ResetPin2()
{
    DigitalPinValues[2] = 0.0f;
}
void AArduino::ResetPin3()
{
    DigitalPinValues[3] = 0.0f;
	DigitalPinValues[2] = 3.0f;
    GetWorldTimerManager().SetTimer(TimerHandle_ResetPin2, this, &AArduino::ResetPin2,1.0f, false);
}

