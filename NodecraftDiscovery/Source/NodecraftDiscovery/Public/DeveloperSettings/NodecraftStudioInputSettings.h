// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "NodecraftStudioInputSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UNodecraftStudioInputSettings& Get() {
		return *GetDefault<UNodecraftStudioInputSettings>();
	}
	
	virtual FName GetCategoryName() const override;


private:
	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	FDataTableRowHandle DefaultLeftThumbstickMenuScrollInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	FDataTableRowHandle DefaultRightThumbstickMenuScrollInputActionData;


public:
	FDataTableRowHandle GetDefaultLeftThumbstickMenuScrollInputActionData() const;
	FDataTableRowHandle GetDefaultRightThumbstickMenuScrollInputActionData() const;
};
