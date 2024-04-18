// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NodecraftStudioStyleSettings.generated.h"


UENUM()
enum class EDateFormat
{
	DayMonthYear		UMETA(DisplayName = "DD/MM/YYYY"),
	MonthDayYear		UMETA(DisplayName = "MM/DD/YYYY")
};


/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioStyleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UNodecraftStudioStyleSettings& Get() {
		return *GetDefault<UNodecraftStudioStyleSettings>();
	}
	virtual FName GetCategoryName() const override;

	TSoftObjectPtr<UTexture2D> GetDefaultAvatarImage() const;
	
	static FString GetDateFormat();
	static FString GetDateWithTimeFormat();

protected:
	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UTexture2D> DefaultAvatarImage;

	UPROPERTY(EditAnywhere, Config)
	EDateFormat DateFormat;
};
