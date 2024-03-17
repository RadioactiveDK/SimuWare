
#include "ArduinoComponent.h"
#include "Arduino.h"
#include "EngineUtils.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AArduinoComponent::AArduinoComponent()
{
   Ard = NULL;
}


// Called when the game starts or when spawned
void AArduinoComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<AArduino> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        // Found an Arduino actor
        Ard = *ActorItr;
        break; // We found the Arduino actor, so exit the loop
    }
}

void AArduinoComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Clamp Input value between 0 and 10
    Input = FMath::Clamp(Input, 0.0f, 10.0f);
	
	if(bHolding && HoldingComp)
	{
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	}


}