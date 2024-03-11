// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/DiscoveryAPISettings.h"

FString UDiscoveryAPISettings::GetApiRoot() const
{
	return ApiRoot;
}

FString UDiscoveryAPISettings::GetPublicGameToken() const
{
	return PublicGameToken;
}

float UDiscoveryAPISettings::GetNotificationPollingInterval() const
{
	return NotificationPollingInterval;
}

float UDiscoveryAPISettings::GetServerJoinPollingInterval() const
{
	return ServerJoinPollingInterval;
}

FName UDiscoveryAPISettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
