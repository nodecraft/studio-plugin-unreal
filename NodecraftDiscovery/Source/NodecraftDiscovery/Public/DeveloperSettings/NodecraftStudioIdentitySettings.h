// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/IdentityType.h"
#include "UObject/Object.h"
#include "NodecraftStudioIdentitySettings.generated.h"

USTRUCT()
struct FIconMapWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EIdentityType, UTexture2D*> IconMap;

};
/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioIdentitySettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	static const UNodecraftStudioIdentitySettings& Get() {
		return *GetDefault<UNodecraftStudioIdentitySettings>();
	}
	
	UPROPERTY(EditAnywhere, Config)
	FIconMapWrapper IconMapWrapper;

	UTexture2D* GetIconForType(EIdentityType Type) const;

	virtual FName GetCategoryName() const override;
};
