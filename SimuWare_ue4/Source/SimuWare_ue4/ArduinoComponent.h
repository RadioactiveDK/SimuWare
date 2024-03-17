#pragma once

#include "CoreMinimal.h"
#include "Pickup.h" 
#include "Arduino.h" 
#include "ArduinoComponent.generated.h"

UCLASS()
class SIMUWARE_UE4_API AArduinoComponent : public APickup 
{
    GENERATED_BODY()

public: 
    // Constructor
    AArduinoComponent();

protected:
    virtual void BeginPlay() override;

public: 
    // Additional functions and overrides
    UPROPERTY(EditAnywhere)
    float Input;

    UPROPERTY(EditAnywhere)
    class AArduino* Ard;

    virtual void Tick(float DeltaTime) override;
};
