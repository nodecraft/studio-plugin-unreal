// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Utility/NodecraftUtility.h"

#include "JsonObjectWrapper.h"
#include "DataTypes/LinkTypes.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"

#define LOCTEXT_NAMESPACE "NodecraftUtility"

FText UNodecraftUtility::ParseMessage(FHttpResponsePtr Response, FString FunctionName)
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

FString UNodecraftUtility::ParseMessageResultCode(FHttpResponsePtr Response)
{
	FString ErrorCode = "Error occurred while trying to parse error code";
	
	if (Response.IsValid())
	{
		FJsonObjectWrapper ResJson;
		
		// Default error message will get used if we don't find 'message' in response
		ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
		ResJson.JsonObject->TryGetStringField("code", ErrorCode);
	}

	return ErrorCode;
}

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

FString UNodecraftUtility::GetQrCodeUrl(const FString& URL, const ELinkType UrlType)
{
	const FString Root = UNodecraftStudioApiSettings::Get().GetApiRoot();

	FString Type = "";
	switch (UrlType)
	{
	case ELinkType::Internal:
		Type = "internal";
		break;
	case ELinkType::External:
		Type = "external";
		break;
	default: ;
	}
	
	return Root + "link/qr/" + Type + "/" + FGenericPlatformHttp::UrlEncode(*URL) + "?content_format=png";
}

FString UNodecraftUtility::GetDefaultServerImageUrl(const FString& ServerId)
{
	return UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/tile";
}

FString UNodecraftUtility::JsonObjToString(TSharedPtr<FJsonObject> JsonObj)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr, 0);
	FJsonSerializer::Serialize(MakeShared<FJsonValueObject>(JsonObj), FString(), JsonWriter);
	return JsonStr;
}

#undef LOCTEXT_NAMESPACE
