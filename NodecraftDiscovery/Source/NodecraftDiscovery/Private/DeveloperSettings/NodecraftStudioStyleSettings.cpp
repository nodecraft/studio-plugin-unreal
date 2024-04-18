// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include <DeveloperSettings/NodecraftStudioStyleSettings.h>

TSoftObjectPtr<UTexture2D> UNodecraftStudioStyleSettings::GetDefaultAvatarImage() const
{
	return DefaultAvatarImage;
}

FString UNodecraftStudioStyleSettings::GetDateFormat()
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

FString UNodecraftStudioStyleSettings::GetDateWithTimeFormat()
{
	switch (Get().DateFormat)
	{
	case EDateFormat::MonthDayYear:
		return "%m/%e/%Y, %H:%M %p";
	case EDateFormat::DayMonthYear:
		return "%e/%m/%Y, %H:%M %p";
	default:
		return "%e/%m/%Y, %H:%M %p";
	}
}

FName UNodecraftStudioStyleSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
