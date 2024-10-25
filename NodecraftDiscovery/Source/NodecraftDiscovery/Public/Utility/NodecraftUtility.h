// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "NodecraftUtility.generated.h"

enum class ELinkType : uint8;
enum class EAlertType;
DECLARE_DELEGATE_TwoParams(FOnRequestAlertMessage, EAlertType /*AlertType*/, FText /*AlertText*/);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftUtility : public UObject
{
	GENERATED_BODY()

public:
	static FText ParseMessage(FHttpResponsePtr Response, FString FunctionName);
	// Parses the 'code' from our response, if it exists
	// This is not the HTTP response code, but a code in the JSON response
	static FString ParseMessageResultCode(FHttpResponsePtr Response);
	static FText TimeLeft(FDateTime Expires);
	static FString GetQrCodeUrl(const FString& URL, const ELinkType UrlType);
	static FString GetDefaultServerImageUrl(const FString& ServerId);
	static FString JsonObjToString(TSharedPtr<FJsonObject> JsonObj);
	static ECommonInputType GetCurrentInputType(const UWorld* World);
};
