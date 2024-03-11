// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/NodecraftUtility.h"

#include "JsonObjectWrapper.h"
#include "Interfaces/IHttpResponse.h"

#define LOCTEXT_NAMESPACE "NodecraftUtility"

FText UNodecraftUtility::ParseError(FHttpResponsePtr Response, FString FunctionName)
{
	FString ErrorMsg = "Error occurred while parsing JSON";
	
	if (Response.IsValid())
	{
		FJsonObjectWrapper ResJson;
		
		// Default error message will get used if we don't find 'message' in response
		ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
		ResJson.JsonObject->TryGetStringField("message", ErrorMsg);
	}

#if UE_BUILD_SHIPPING
	return FText::FromString(ErrorMsg);
#else
	return FText::FromString(FString::Printf(TEXT("%ls: Response code: %d, Message: %ls"), *FunctionName, Response.Get()->GetResponseCode(), *ErrorMsg));
#endif
}

/*
export function timeLeft(date: Date | null) {
	if (date === null) {
		return 'Permanent';
	}
	const now = new Date();
	const diff = date.getTime() - now.getTime();
	if (diff < 0) {
		return 'Expired';
	}
	const seconds = Math.floor(diff / 1000);
	const minutes = Math.floor(seconds / 60);
	const hours = Math.floor(minutes / 60);
	const days = Math.floor(hours / 24);
	if (days > 7) {
		return date.toLocaleDateString();
	} else if (days > 0) {
		return new Intl.RelativeTimeFormat('en', { numeric: 'auto' }).format(days, 'day');
	} else if (hours > 0) {
		return new Intl.RelativeTimeFormat('en', { numeric: 'auto' }).format(hours, 'hour');
	} else if (minutes > 0) {
		return new Intl.RelativeTimeFormat('en', { numeric: 'auto' }).format(minutes, 'minute');
	} else if (seconds > 0) {
		return new Intl.RelativeTimeFormat('en', { numeric: 'auto' }).format(seconds, 'second');
	}
}
*/
FText UNodecraftUtility::TimeLeft(const FDateTime Expires)
{
	if (Expires == FDateTime::MinValue())
	{
		return FText::FromString("Permanent");
	}
	else
	{
		// Copy the functionality from the JavaScript function above
		const FDateTime Now = FDateTime::UtcNow();
		const FTimespan Diff = Expires - Now;
		if (Diff.GetTicks() <= 0)
		{
			return FText::FromString("Expired");
		}
		const int64 Seconds = Diff.GetSeconds();
		const int64 Minutes = Diff.GetMinutes();
		const int64 Hours = Diff.GetHours();
		const int64 Days = Diff.GetDays();
		if (Diff.GetTotalDays() > 7)
		{
			return FText::AsDate(Expires,EDateTimeStyle::Short);
		}
		// TODO: This would be more localized if we use FText::AsTimespan, but would change formatting
		else if (Days > 0)
		{
			return FText::Format(LOCTEXT("TimeLeftDays", "{0} {0}|plural(one=day,other=days)"), Days);
		}
		else if (Hours > 0)
		{
			return FText::Format(LOCTEXT("TimeLeftHours", "{0} {0}|plural(one=hour,other=hours)"), Hours);
		}
		else if (Minutes > 0)
		{
			return FText::Format(LOCTEXT("TimeLeftMinutes", "{0} {0}|plural(one=minute,other=minutes)"), Minutes);
		}
		else
		{
			return FText::Format(LOCTEXT("TimeLeftSeconds", "{0} {0}|plural(one=second,other=seconds)"), Seconds);
		}
	}
}

#undef LOCTEXT_NAMESPACE