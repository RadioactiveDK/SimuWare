// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "GameFramework/Character.h"
#include "SimuWare_ue4Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	ItemMesh->SetSimulatePhysics(true);
	RootComponent = ItemMesh;

	bHolding = false;
	bGravity = true;

}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;
 
	MyCharacter->GetComponents(Components);

	if(Components.Num() > 0)
	{
		for (auto& Comp : Components)
		{
			if(Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bool isFlying = false;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ASimuWare_ue4Character* SimuWareCharacter = Cast<ASimuWare_ue4Character>(PlayerCharacter);

    if (SimuWareCharacter)
    {
        isFlying = SimuWareCharacter->bIsflying;
    }
	if(bHolding && HoldingComp)
	{
		FVector TargetLocation = HoldingComp->GetComponentLocation();
		if(isFlying==false){
			SetActorLocationAndRotation(TargetLocation, HoldingComp->GetComponentRotation());
			return;
		}
		int32 GridX = FMath::GridSnap(TargetLocation.X ,100.0f);
        int32 GridY = FMath::GridSnap(TargetLocation.Y ,100.0f);
        int32 GridZ = FMath::GridSnap(TargetLocation.Z ,100.0f);

        FVector Location = FVector(GridX,
                                        GridY,
                                        GridZ);
		
		SetActorLocationAndRotation(Location, FRotator::ZeroRotator);
	}

}

void APickup::RotateActor(){
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));
}

void APickup::Pickup(){
	bHolding = !bHolding;	
	bGravity = !bGravity;
	ItemMesh->SetEnableGravity(bGravity);
	ItemMesh->SetSimulatePhysics(bHolding ? false : true);
	ItemMesh->SetCollisionEnabled(bHolding ? (ECollisionEnabled::NoCollision) : (ECollisionEnabled::QueryAndPhysics));

	if(!bHolding) 
	{
		ForwardVector = PlayerCamera->GetForwardVector()*100;
		//ItemMesh->AddForce(ForwardVector*100000*ItemMesh->GetMass());
	}
}