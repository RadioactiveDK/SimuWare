// Fill out your copyright notice in the Description page of Project Settings.


#include "LED.h"
#include "Materials/MaterialInstanceDynamic.h"
// Sets default values
ALED::ALED()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LED_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LED Mesh"));
	LED_Mesh->SetupAttachment(ItemMesh);
}

// Called when the game starts or when spawned
void ALED::BeginPlay()
{
	Super::BeginPlay();
	
    Material = ItemMesh->GetMaterial(0);
	Input = 0.0f;
	CreateAndApplyDynamicMaterial(Input);
}

// Called every frame
void ALED::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Input += 0.1f ;
	if (Input > 5.0f)
	{
		CreateAndApplyDynamicMaterial(Input);
	}
	if(Input>9.0f){
		Input = 0;
		CreateAndApplyDynamicMaterial(Input);
	}
}

void ALED::CreateAndApplyDynamicMaterial(float val)
{
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material,this);
	
	if(!DynMaterial) return;
	
	ItemMesh->SetMaterial(0,DynMaterial);
	LED_Mesh->SetMaterial(0,DynMaterial);
	
	DynMaterial->SetScalarParameterValue("Emissive_Strength",20);

	if(val<5.0f)
	DynMaterial->SetVectorParameterValue("LED_Color",FLinearColor::Yellow);
	else 
	DynMaterial->SetVectorParameterValue("LED_Color",FLinearColor::Red);
}