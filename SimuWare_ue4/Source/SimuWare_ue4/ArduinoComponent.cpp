
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
        Ard = *ActorItr;
        break;
    }
}

void AArduinoComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
    Input = FMath::Clamp(Input, 0.0f, 10.0f);
}