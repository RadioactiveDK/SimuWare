#pragma once

#include "CoreMinimal.h"
#include "ArduinoComponent.h"
#include "Motor.generated.h"

UCLASS()
class SIMUWARE_UE4_API AMotor : public AArduinoComponent
{
	GENERATED_BODY()
		
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* MotorBody;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Rotor;
    
    UPROPERTY(VisibleAnywhere)
    class UPhysicsConstraintComponent* TwistConstraint;

public:	
	// Sets default values for this actor's properties
	AMotor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};