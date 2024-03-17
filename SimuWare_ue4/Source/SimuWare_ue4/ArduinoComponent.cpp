
#include "ArduinoComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AArduinoComponent::AArduinoComponent()
{
   
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

void AArduinoComponent::Increase()
{
	Input = Input + 1.0f;
}

void AArduinoComponent::Decrease()
{
	Input = Input - 1.0f;
}
