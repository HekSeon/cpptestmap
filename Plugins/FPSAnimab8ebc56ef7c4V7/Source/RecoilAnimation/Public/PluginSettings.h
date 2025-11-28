// Designed by KINEMATION, 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PluginSettings.generated.h"

UCLASS(config = PluginSetting)
class RECOILANIMATION_API UPluginSettings : public UObject
{
	GENERATED_BODY()

public:
	UPluginSettings(const FObjectInitializer& Obj);

	/** Default location for curves */
	UPROPERTY(Config, EditAnywhere, Category = "Essentials")
	FString CurvesSavePath{"/PRAS/Curves/"};
};