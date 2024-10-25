// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"

UTexture2D* UNodecraftStudioIdentitySettings::GetIconForType(EIdentityType Type) const
{
	return *IconMapWrapper.IconMap.Find(Type);
}

FName UNodecraftStudioIdentitySettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
