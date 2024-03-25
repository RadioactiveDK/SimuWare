// Copyright Epic Games, Inc. All Rights Reserved.

#include "Kismet/GameplayStatics.h"
#include "SimuWare_ue4Character.h"
#include "SimuWare_ue4Projectile.h"
#include "Item.h"
#include "Pickup.h"
#include "LED.h"
#include "Arduino.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASimuWare_ue4Character

ASimuWare_ue4Character::ASimuWare_ue4Character()
{
	ItemIdx = 0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false); // otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false); // otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f)); // Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	// bUsingMotionControllers = true;

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	// HoldingComponent->RelativeLocation.X = 50.0f;
	HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 50.0f));
	HoldingComponent->SetupAttachment(FP_MuzzleLocation);

	CurrentItem = NULL;
	CurrentLED = NULL;
	bInspecting = false;
	bArdItem = false;
	bIsflying = false;
}

void ASimuWare_ue4Character::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	// Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	PitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	PitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
}

void ASimuWare_ue4Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Start = FirstPersonCameraComponent->GetComponentLocation();
	ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	End = ((ForwardVector * 200.f) + Start);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
		{
			if (Hit.GetActor()->GetClass()->IsChildOf(APickup::StaticClass()))
			{
				CurrentItem = Cast<APickup>(Hit.GetActor());
			}
			if (Hit.GetActor()->GetClass()->IsChildOf(ALED::StaticClass()))
			{
				CurrentLED = Cast<ALED>(Hit.GetActor());
			}
			if(Hit.GetActor()->GetClass()->IsChildOf(AArduino::StaticClass()))
			{
				Ard = Cast<AArduino>(Hit.GetActor());
				bArdItem =true;
			}
		}
		else
		{
			CurrentItem = NULL;
			CurrentLED = NULL;
			Ard = NULL;
			bArdItem = false;
		}
	}

	if (bInspecting)
	{
		if (bHoldingItem)
		{
			FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 150.0f, 50.0f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.9000002f;
			CurrentItem->RotateActor();
		}
		else
		{
			FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 45.0f, 0.1f));
		}
	}
	else
	{
		FirstPersonCameraComponent->SetFieldOfView(FMath::Lerp(FirstPersonCameraComponent->FieldOfView, 90.0f, 0.1f));

		if (bHoldingItem)
		{
			HoldingComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 50.0f));
		}
	}
	const FRotator SpawnRotation = GetControlRotation();
	UpdatePreviewMesh(((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset), 200.0f);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASimuWare_ue4Character::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ASimuWare_ue4Character::OnGrab);
	PlayerInputComponent->BindAction("OpenIDE", IE_Pressed, this, &ASimuWare_ue4Character::OpenIDE);
	PlayerInputComponent->BindAction("Inspect", IE_Pressed, this, &ASimuWare_ue4Character::OnInspect);
	PlayerInputComponent->BindAction("Inspect", IE_Released, this, &ASimuWare_ue4Character::OnInspectReleased);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASimuWare_ue4Character::OnFire);

	PlayerInputComponent->BindAction("ItemUp", IE_Pressed, this, &ASimuWare_ue4Character::ItemUp);
	PlayerInputComponent->BindAction("ItemDown", IE_Pressed, this, &ASimuWare_ue4Character::ItemDown);
	PlayerInputComponent->BindAction("DeployItem", IE_Pressed, this, &ASimuWare_ue4Character::DeployItem);
	PlayerInputComponent->BindAction("DeleteItem", IE_Pressed, this, &ASimuWare_ue4Character::DeleteItem);
	PlayerInputComponent->BindAction("Resize_1x", IE_Pressed, this, &ASimuWare_ue4Character::Resize_1x);
	PlayerInputComponent->BindAction("Resize_2x", IE_Pressed, this, &ASimuWare_ue4Character::Resize_2x);
	PlayerInputComponent->BindAction("Resize_4x", IE_Pressed, this, &ASimuWare_ue4Character::Resize_4x);

	// change input
	PlayerInputComponent->BindAction("IncreaseInput", IE_Pressed, this, &ASimuWare_ue4Character::IncreaseInput);
	PlayerInputComponent->BindAction("DecreaseInput", IE_Pressed, this, &ASimuWare_ue4Character::DecreaseInput);
	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASimuWare_ue4Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASimuWare_ue4Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASimuWare_ue4Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASimuWare_ue4Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASimuWare_ue4Character::LookUpAtRate);
	// Flying mode
	PlayerInputComponent->BindAxis("MoveUp", this, &ASimuWare_ue4Character::MoveUp);
	PlayerInputComponent->BindAction("EnterFlight", IE_Pressed, this, &ASimuWare_ue4Character::EnterFlight);
}

void ASimuWare_ue4Character::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld *const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ASimuWare_ue4Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				// Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<ASimuWare_ue4Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance *AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ASimuWare_ue4Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASimuWare_ue4Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASimuWare_ue4Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

// Commenting this section out to be consistent with FPS BP template.
// This allows the user to turn without using the right virtual joystick

// void ASimuWare_ue4Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
// }

void ASimuWare_ue4Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASimuWare_ue4Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASimuWare_ue4Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASimuWare_ue4Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ASimuWare_ue4Character::EnableTouchscreenMovement(class UInputComponent *PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASimuWare_ue4Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASimuWare_ue4Character::EndTouch);

		// Commenting this out to be more consistent with FPS BP template.
		// PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASimuWare_ue4Character::TouchUpdate);
		return true;
	}

	return false;
}


// Function to update the preview mesh's position and size based on the target location and mesh size
void ASimuWare_ue4Character::UpdatePreviewMesh(const FVector& TargetLocation, float GridSize)
{
    if (PreviewMesh)
    {
        // Calculate the grid position for the target location
        int32 GridX = FMath::GridSnap(TargetLocation.X ,GridSize);
        int32 GridY = FMath::GridSnap(TargetLocation.Y ,GridSize);
        int32 GridZ = FMath::GridSnap(TargetLocation.Z ,GridSize);

        // Previewlocation the corresponding grid cell
        FVector PreviewLocation = FVector(GridX,
                                          GridY,
                                          GridZ);

        // Set the preview mesh's position
        PreviewMesh->SetActorLocation(PreviewLocation);
    }
}
void ASimuWare_ue4Character::CreatePreviewMesh()
{
    UWorld* const World = GetWorld();
    if (World && Inventory[ItemIdx] != nullptr)
    {
		
			// Set Spawn Collision Handling Override
			const FRotator SpawnRotation = GetControlRotation();
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
            // Spawn an instance of the inventory item
            PreviewMesh =  World->SpawnActor<AItem>(Inventory[ItemIdx],((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset), FRotator(0.0f,0.0f,0.0f), ActorSpawnParams);
            if (PreviewMesh)
            {
                // Access the ItemMesh from the temporary instance
                UStaticMeshComponent* ItemMesh = PreviewMesh->ItemMesh;
                if (ItemMesh)
                {
                    // Access the StaticMesh from the ItemMesh
                    UStaticMesh* StaticMesh = ItemMesh->GetStaticMesh();
                    if (StaticMesh)
                    {
                        // Create the preview mesh component
                        // PreviewMesh = NewObject<UStaticMeshComponent>(this);
                        // if (PreviewMesh)
                        // {
                        //     PreviewMesh->SetStaticMesh(StaticMesh);
                        //     PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                        //     PreviewMesh->SetVisibility(true);
                        //     PreviewMesh->RegisterComponent();
                        // }
                    }
                }
            }
        }
}
void ASimuWare_ue4Character::RemovePreviewMesh()
{
    if (PreviewMesh)
    {
        PreviewMesh->Destroy();
        PreviewMesh = nullptr;
    }
}

void ASimuWare_ue4Character::DeployItem()
{
	if (Inventory[ItemIdx] != nullptr)
	{
		UWorld *const World = GetWorld();

		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		ASimuWare_ue4Character* FirstPersonCharacter = Cast<ASimuWare_ue4Character>(PlayerCharacter);
		FVector TargetLocation = FirstPersonCharacter->VR_MuzzleLocation->GetComponentLocation();
		float GridSize = 100.0f;

		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				int32 GridX = FMath::GridSnap(TargetLocation.X ,GridSize);
                int32 GridY = FMath::GridSnap(TargetLocation.Y ,GridSize);
                int32 GridZ = FMath::GridSnap(TargetLocation.Z ,GridSize);

                // Spawn location at corresponding grid
                FVector SpawnLocation = FVector(GridX,
                                             GridY,
                                             GridZ);
				RemovePreviewMesh();
		
				if(bIsflying==false){
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					// Spawn the item
					AItem* SpawnedItem = World->SpawnActor<AItem>(Inventory[ItemIdx], TargetLocation, FRotator::ZeroRotator, ActorSpawnParams);
					return;
				}
				

				// Set up collision parameters
				FActorSpawnParameters ActorSpawnParams;

                FCollisionQueryParams CollisionParams;
                CollisionParams.AddIgnoredActor(this); // Ignore the character itself

                // Set up the shape of the sweep (box shape centered at the spawn location)
                FVector HalfSize(24.9f, 24.9f, 24.9f); // Half size of the box
                FQuat Rotation = FQuat::Identity; // No rotation
                FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);

                // Perform a sweep to check for overlapping items
                FHitResult HitResult;
                bool bHit = World->SweepSingleByChannel(HitResult, SpawnLocation, SpawnLocation, Rotation, ECC_WorldStatic, CollisionShape, CollisionParams);

				// spawn the projectile at the muzzle
				if(!bHit){
					AItem* SpawnedItem = World->SpawnActor<AItem>(Inventory[ItemIdx], SpawnLocation, FRotator(0.0f,0.0f,0.0f), ActorSpawnParams);
				}
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();

			    TargetLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				int32 GridX = FMath::GridSnap(TargetLocation.X ,GridSize);
                int32 GridY = FMath::GridSnap(TargetLocation.Y ,GridSize);
                int32 GridZ = FMath::GridSnap(TargetLocation.Z ,GridSize);

                // Spawn location at corresponding grid
                FVector SpawnLocation = FVector(GridX,
                                             GridY,
                                             GridZ);

				if(bIsflying==false){
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					// Spawn the item
					AItem* SpawnedItem = World->SpawnActor<AItem>(Inventory[ItemIdx], TargetLocation, FRotator::ZeroRotator, ActorSpawnParams);
					return;
				}

				
				// Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				
				RemovePreviewMesh();
				
				// Set up collision parameters
                FCollisionQueryParams CollisionParams;
                CollisionParams.AddIgnoredActor(this); // Ignore the character itself

                // Set up the shape of the sweep (box shape centered at the spawn location)
                FVector HalfSize(24.9f, 24.9f, 24.9f); // Half size of the box
                FQuat Rotation = FQuat::Identity; // No rotation
                FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);

                // Perform a sweep to check for overlapping items
                FHitResult HitResult;
                bool bHit = World->SweepSingleByChannel(HitResult, SpawnLocation, SpawnLocation, Rotation, ECC_WorldStatic, CollisionShape, CollisionParams);

				// spawn the projectile at the muzzle
				if(!bHit){
					AItem* SpawnedItem = World->SpawnActor<AItem>(Inventory[ItemIdx], SpawnLocation, FRotator(0.0f,0.0f,0.0f), ActorSpawnParams);
					
				if(SpawnedItem)
				{
					UStaticMeshComponent* ItemMesh = SpawnedItem->ItemMesh;
				    if (ItemMesh)
				    {
				    	UStaticMesh* StaticMesh = ItemMesh->GetStaticMesh();
				    	if (StaticMesh)
				    	{
				    		FBoxSphereBounds Bounds = StaticMesh->GetBoundingBox();
				    		 //  accessing the bounding box of the mesh
				    		FVector Origin = Bounds.Origin;
				    		FVector BoxExtent = Bounds.BoxExtent;
				    		// Get the size of the mesh
				    		FVector Extent = Bounds.GetBox().GetExtent();
				    		GridSize= Extent.X * 2.0f; // Multiply by 2 to get the full size
				    		GridX = FMath::RoundToFloat(TargetLocation.X / GridSize) * GridSize;
				    		GridSize = Extent.Y * 2.0f;
				    		GridY = FMath::RoundToFloat(TargetLocation.Y / GridSize) * GridSize;
				    		GridSize = Extent.Z * 2.0f;
				    		GridZ = FMath::RoundToFloat(TargetLocation.Z / GridSize) * GridSize;
				    		//SpawnedItem->SetActorLocation(FVector(GridX, GridY, GridZ));
				    	}
				    }
				}
				}
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void ASimuWare_ue4Character::ItemUp()
{
	ItemIdx = (ItemIdx + 1) % 8;
}

void ASimuWare_ue4Character::ItemDown()
{
	if (ItemIdx == 0)ItemIdx = 8;
	ItemIdx--;
}

void ASimuWare_ue4Character::MoveUp(float Value)
{
	if ((GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying) && (Value != 0.0f))
	{
		const auto upVector = GetActorUpVector();

		AddMovementInput(upVector, Value);
	}
}
void ASimuWare_ue4Character::EnterFlight()
{
	switch (GetCharacterMovement()->MovementMode)
	{
	case EMovementMode::MOVE_Swimming:
		break;
	case EMovementMode::MOVE_Flying:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		RequestFlight(false);
		break;
	case EMovementMode::MOVE_Falling:
	case EMovementMode::MOVE_NavWalking:
	case EMovementMode::MOVE_Walking:
		GetCharacterMovement()->bCheatFlying = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		RequestFlight(true);
		break;
	default:
		break;
	}
}
void ASimuWare_ue4Character::ExitFlight()
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		RequestFlight(false);
	}
}
void ASimuWare_ue4Character::RequestFlight(bool bWantsToFly)
{
	switch (GetCharacterMovement()->MovementMode)
	{

	case EMovementMode::MOVE_Swimming:
		break;
	case EMovementMode::MOVE_Flying:
		if (!bWantsToFly)
		{
			GetCharacterMovement()->bCheatFlying = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			bIsflying = false;
		}
		break;
	case EMovementMode::MOVE_Falling:
		if (bWantsToFly)
		{
			GetCharacterMovement()->bCheatFlying = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		}
		break;
	case EMovementMode::MOVE_NavWalking:
	case EMovementMode::MOVE_Walking:
		if (bWantsToFly)
		{
			GetCharacterMovement()->bCheatFlying = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			bIsflying = true;
		}
		break;
	default:
		break;
	}
}

void ASimuWare_ue4Character::OnGrab()
{
	if (CurrentItem && !bInspecting)
	{
		ToggleItemPickup();
	}
}

void ASimuWare_ue4Character::OpenIDE()
{
	if(Ard && !bHoldingItem)
	{
		;
	}
}

void ASimuWare_ue4Character::OnInspect()
{
	if (bHoldingItem)
	{
		LastRotation = GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bInspecting = true;
	}
}

void ASimuWare_ue4Character::OnInspectReleased()
{
	if (bInspecting && bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotation);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = PitchMin;
		ToggleMovement();
	}
	else
	{
		bInspecting = false;
	}
}

void ASimuWare_ue4Character::ToggleMovement()
{
	bInspecting = !bInspecting;
	FirstPersonCameraComponent->bUsePawnControlRotation = !FirstPersonCameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
}

void ASimuWare_ue4Character::ToggleItemPickup()
{
	if (CurrentItem)
	{
		bHoldingItem = !bHoldingItem;
		CurrentItem->Pickup();

		if (!bHoldingItem)
		{
			CurrentItem = NULL;
		}
	}
}

void ASimuWare_ue4Character::DeleteItem()
{
	if (CurrentItem)
	{
		CurrentItem->Destroy();
	}
}
void ASimuWare_ue4Character::Resize_1x()
{
		if(CurrentItem && bInspecting && bHoldingItem){
				CurrentItem->SetActorScale3D(FVector(0.5,0.5,0.5));		
	}
}

void ASimuWare_ue4Character::Resize_2x()
{
		if(CurrentItem && bInspecting && bHoldingItem){
				CurrentItem->SetActorScale3D(FVector(1,1,1));			
		}
}

void ASimuWare_ue4Character::Resize_4x()
{
		if(CurrentItem && bInspecting && bHoldingItem){
				CurrentItem->SetActorScale3D(FVector(1.5,1.5,1.5));			
		}
}

void ASimuWare_ue4Character::IncreaseInput()
{
	if(CurrentLED)
	{
		CurrentLED->Input += 1.0f;
	}
}

void ASimuWare_ue4Character::DecreaseInput()
{
	if(CurrentLED)
	{
		CurrentLED->Input -= 1.0f;
	}
}
