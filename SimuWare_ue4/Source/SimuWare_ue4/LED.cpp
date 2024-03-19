// Fill out your copyright notice in the Description page of Project Settings.


#include "LED.h"
#include "Arduino.h"
#include "Materials/MaterialInstanceDynamic.h"
// Sets default values
ALED::ALED()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// LED_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED Mesh"));
	// LED_Mesh->SetupAttachment(ItemMesh);
    // LED_Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ALED::BeginPlay()
{
	Super::BeginPlay();
	
    Material = ItemMesh->GetMaterial(0);
	CreateAndApplyDynamicMaterial(Input);
}

// Called every frame
void ALED::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Input = Ard->DigitalPinValues[pinNumber2] - Ard->DigitalPinValues[pinNumber1];
	
	DynMaterial->SetScalarParameterValue("Emissive_Strength",Input*5);
}

void ALED::CreateAndApplyDynamicMaterial(float val)
{
	DynMaterial = UMaterialInstanceDynamic::Create(Material,this);
	if(!DynMaterial) return;
	
	ItemMesh->SetMaterial(0,DynMaterial);
	// LED_Mesh->SetMaterial(0,DynMaterial);
	
	DynMaterial->SetScalarParameterValue("Emissive_Strength",20);
	DynMaterial->SetVectorParameterValue("LED_Color",FLinearColor::Green);
}