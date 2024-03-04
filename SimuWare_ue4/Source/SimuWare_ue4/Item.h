// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "Item.generated.h"

// class UMeshComponent;

UCLASS()
class SIMUWARE_UE4_API AItem : public APickup
{
	GENERATED_BODY()
		
	// UPROPERTY(EditAnywhere)
	// UMeshComponent* ItemMesh;
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// UMeshComponent* GetMesh() const { return ItemMesh; }
};