#pragma once

#include "CoreMinimal.h"
#include "Pickup.h" 
#include "ArduinoComponent.generated.h"

UCLASS()
class SIMUWARE_UE4_API AArduinoComponent : public APickup 
{
    GENERATED_BODY()

public: 
    // Constructor
    AArduinoComponent();

    // Additional functions and overrides
    UPROPERTY(EditAnywhere)
    float Input;

    virtual void Tick(float DeltaTime) override;
};
