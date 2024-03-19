#include "ArduinoIDEWidget.h"
#include "Misc/FileHelper.h"

void UArduinoIDEWidget::SaveCodeToFile(FString CodeContent)
{
    FFileHelper::SaveStringToFile(CodeContent, *CodeFilePath);
}

FString UArduinoIDEWidget::LoadCodeFromFile()
{
    FString CodeContent;
    FFileHelper::LoadFileToString(CodeContent, *CodeFilePath);
    return CodeContent;
}

void UArduinoIDEWidget::CompileAndUploadCode()
{

}
