#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArduinoIDEWidget.generated.h"

UCLASS()
class YOURPROJECT_API UArduinoIDEWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SaveCodeToFile(FString CodeContent);

    UFUNCTION(BlueprintCallable)
    FString LoadCodeFromFile();

    UFUNCTION(BlueprintCallable)
    void CompileAndUploadCode();

private:
    FString CodeFilePath = FPaths::ProjectDir() + "SavedCode.txt";
};
