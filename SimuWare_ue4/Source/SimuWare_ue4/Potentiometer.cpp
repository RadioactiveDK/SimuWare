// Fill out your copyright notice in the Description page of Project Settings.


#include "Potentiometer.h"

// Sets default values
APotentiometer::APotentiometer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PotentiometerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Potentiometer Mesh"));
	RootComponent = PotentiometerMesh;

}

// Called when the game starts or when spawned
void APotentiometer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotentiometer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float OutputValue = AmplificationFactor * Input;

}

