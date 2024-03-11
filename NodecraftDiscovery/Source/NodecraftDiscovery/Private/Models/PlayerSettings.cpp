// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/PlayerSettings.h"

UPlayerSettings* UPlayerSettings::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UPlayerSettings* PlayerSettings = NewObject<UPlayerSettings>();

	PlayerSettings->AnalyticsOptOut = Json->GetBoolField("analytics_opt_out");
	PlayerSettings->DefaultServerRegionID = Json->GetStringField("default_server_region_id");

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
