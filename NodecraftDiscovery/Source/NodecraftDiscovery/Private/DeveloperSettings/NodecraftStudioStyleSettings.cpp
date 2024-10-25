// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "DeveloperSettings/NodecraftStudioStyleSettings.h"

TSoftObjectPtr<UTexture2D> UNodecraftStudioStyleSettings::GetDefaultAvatarImage() const
{
	return DefaultAvatarImage;
}

FName UNodecraftStudioStyleSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
