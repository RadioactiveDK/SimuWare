#pragma once

#include "CoreMinimal.h"
#include "ArduinoComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "PotentiometerWidget.generated.h"

UCLASS()
class SIMUWARE_UE4_API UPotentiometerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    USlider* PotentiometerSlider;

    UFUNCTION()
    void OnValueChanged(float NewValue);
};
