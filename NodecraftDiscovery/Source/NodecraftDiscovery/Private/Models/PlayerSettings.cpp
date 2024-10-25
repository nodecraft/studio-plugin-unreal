// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/PlayerSettings.h"

UPlayerSettings* UPlayerSettings::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UPlayerSettings* PlayerSettings = NewObject<UPlayerSettings>();

	Json->TryGetBoolField(TEXT("analytics_opt_out"), PlayerSettings->AnalyticsOptOut);
	Json->TryGetStringField(TEXT("default_server_region_id"), PlayerSettings->DefaultServerRegionID);

	return PlayerSettings;
}

FString UPlayerSettings::GetDefaultServerRegionID() const
{
	return DefaultServerRegionID;
}

bool UPlayerSettings::GetAnalyticsOptOut() const
{
	return AnalyticsOptOut;
}
