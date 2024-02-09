// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SimuWare_ue4HUD.generated.h"

UCLASS()
class ASimuWare_ue4HUD : public AHUD
{
	GENERATED_BODY()

public:
	ASimuWare_ue4HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

