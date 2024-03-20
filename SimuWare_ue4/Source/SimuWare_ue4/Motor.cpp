#include "Motor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/UnrealMathUtility.h"

AMotor::AMotor(){
    
    Input = 5.0f;

    // MotorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotorBody"));
    // MotorBody->SetupAttachment(ItemMesh);
    const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
    ItemMesh->SetStaticMesh(MeshObj.Object);

    
    // Create Rotor component and set its properties
    Rotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rotor"));
    Rotor->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object);
    Rotor->SetupAttachment(RootComponent);

    FVector relativeLocation = FVector(0.0f, 0.0f, 70.0f);
    Rotor->SetRelativeLocation(relativeLocation);
    Rotor->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.4f));

    // Create TwistConstraint component and set its properties
    TwistConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TwistConstraint"));
    TwistConstraint->SetupAttachment(Rotor);
    TwistConstraint->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));

    
    //physics simulation
    Rotor->SetSimulatePhysics(true);
    ItemMesh->SetSimulatePhysics(false);

    // Set angular drive mode
    TwistConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
    TwistConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 180.0f);
    TwistConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 0.0f);
    TwistConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 0.0f);
    TwistConstraint->SetDisableCollision(true);

    //Damping
    TwistConstraint->SetAngularDriveParams(500.0f, 50.0f, 0.0f);

}

void AMotor::BeginPlay()
{
    Super::BeginPlay();
}

void AMotor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  
  // Get the transform of the Rotor component
  FTransform RotorTransform = Rotor->GetComponentTransform();

  // Get the local Z-axis vector from the rotation part of the transform
  FVector LocalVector = RotorTransform.GetUnitAxis(EAxis::Z);

  // Now you have the direction of the local Z-axis of the Rotor in world space
  // You can use LocalZVector as needed

  // For example, you can apply torque based on this direction
  Rotor->AddTorqueInDegrees(LocalVector * Input, NAME_None, true);
}
