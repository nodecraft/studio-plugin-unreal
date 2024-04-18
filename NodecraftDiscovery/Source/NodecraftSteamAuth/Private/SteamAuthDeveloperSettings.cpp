// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "SteamAuthDeveloperSettings.h"

FString USteamAuthDeveloperSettings::GetAppId() const
{
	return AppId;
}

FName USteamAuthDeveloperSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
