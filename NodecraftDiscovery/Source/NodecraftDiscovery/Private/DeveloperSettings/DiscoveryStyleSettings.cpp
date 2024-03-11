// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/DiscoveryStyleSettings.h"

TSoftObjectPtr<UTexture2D> UDiscoveryStyleSettings::GetDefaultAvatarImage() const
{
	return DefaultAvatarImage;
}

FString UDiscoveryStyleSettings::GetDateFormat()
{
	switch (Get().DateFormat)
	{
	case EDateFormat::MonthDayYear:
		return "%m/%e/%Y";
	case EDateFormat::DayMonthYear:
		return "%e/%m/%Y";
	default:
		return "%e/%m/%Y";
	}
}

FName UDiscoveryStyleSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
