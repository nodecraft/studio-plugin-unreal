// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/Texture2D.h"

#include "NodecraftStudioStyleSettings.generated.h"


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

protected:
	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftObjectPtr<UTexture2D> DefaultAvatarImage;
};
