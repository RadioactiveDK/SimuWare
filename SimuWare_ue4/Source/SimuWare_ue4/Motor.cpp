#include "Motor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
AMotor::AMotor(){

}
void AMotor::BeginPlay()
{
    Super::BeginPlay();

    // MotorBody component and its properties
    MotorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotorBody"));
    MotorBody->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object);
    RootComponent = MotorBody;

    // Create Rotor component and set its properties
    Rotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor"));
    Rotor->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object);
    Rotor->SetupAttachment(MotorBody);

    // Create TwistConstraint component and set its properties
    TwistConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TwistConstraint"));
    TwistConstraint->SetupAttachment(MotorBody);
    Rotor->SetSimulatePhysics(true);
    MotorBody->SetSimulatePhysics(false);

    //TwistConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free);
    // Set angular drive mode
    TwistConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);

    //Damping
    TwistConstraint->SetAngularDriveParams(500.0f, 50.0f, 0.0f);


    // Disable collision for the Rotor
    Rotor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
