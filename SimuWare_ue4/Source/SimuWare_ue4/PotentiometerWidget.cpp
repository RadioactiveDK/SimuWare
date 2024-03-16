#include "PotentiometerWidget.h"

void UPotentiometerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PotentiometerSlider = Cast<USlider>(GetWidgetFromName(TEXT("PotentiometerSlider")));

    if (PotentiometerSlider)
    {
        PotentiometerSlider->OnValueChanged.AddDynamic(this, &UPotentiometerWidget::OnValueChanged);
    }
}

void UPotentiometerWidget::OnValueChanged(float NewValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Potentiometer value: %f"), NewValue);
}
