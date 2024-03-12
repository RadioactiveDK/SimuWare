// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArduinoComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "LED.generated.h"


UCLASS()
class SIMUWARE_UE4_API ALED : public AArduinoComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALED();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LED_Mesh;

	UMaterialInterface* Material;
    void CreateAndApplyDynamicMaterial(float val);
};
