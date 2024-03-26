#include "PotentiometerWidget.h"

void UPotentiometerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Define the class reference for PotentiometerWidget
    FStringClassReference PotentiometerWidgetClassRef(TEXT("/Game/PotentiometerWidgetClass.PotentiometerWidgetClass"));
    // WidgetBlueprint'/Game/PotentiometerWidgetClass.PotentiometerWidgetClass'
    // Load the class reference
    TSubclassOf<UPotentiometerWidget> PotentiometerWidgetClass = PotentiometerWidgetClassRef.TryLoadClass<UPotentiometerWidget>();


    PotentiometerSlider = Cast<USlider>(GetWidgetFromName(TEXT("PotentiometerSlider")));

    if (PotentiometerSlider)
    {
        PotentiometerSlider->OnValueChanged.AddDynamic(this, &UPotentiometerWidget::OnValueChanged);
    }
}

UPotentiometerWidget::UPotentiometerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PotentiometerSlider = ObjectInitializer.CreateDefaultSubobject<USlider>(this, TEXT("PotentiometerSlider"));

}


void UPotentiometerWidget::OnValueChanged(float NewValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Potentiometer value: %f"), NewValue);
}
