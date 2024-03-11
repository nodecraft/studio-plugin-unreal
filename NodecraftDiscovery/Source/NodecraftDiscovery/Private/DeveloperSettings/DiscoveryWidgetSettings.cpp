// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/DiscoveryWidgetSettings.h"

FName UDiscoveryWidgetSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}

TSoftClassPtr<UCommonActivatableWidget> UDiscoveryWidgetSettings::GetJoiningServerQueueModal() const
{
	return JoiningServerQueueModal;
}

TSoftClassPtr<UCommonActivatableWidget> UDiscoveryWidgetSettings::GetServerPasswordModal() const
{
	return ServerPasswordModal;
}
