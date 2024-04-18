// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Subsystems/EngineSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "MessageRouterSubsystem.generated.h"


DECLARE_DELEGATE_ThreeParams(FOnRoutedMessageReceived, const FString& /*MessageCode*/, const FText& /*Message*/, EAlertType);

UCLASS()
class NODECRAFTDISCOVERY_API UMessageRouterSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	struct FErrorRoute
	{
		FErrorRoute(const FString& Id, const TArray<FString>& Codes, const FOnRoutedMessageReceived& Delegate)
			: Id(Id),
			  Codes(Codes),
			  Delegate(Delegate)
		{
		}

		FString Id;
		TArray<FString> Codes;
		FOnRoutedMessageReceived Delegate;
	};

public:
	static UMessageRouterSubsystem& Get() { return *GEngine->GetEngineSubsystem<UMessageRouterSubsystem>(); }

	void RouteMessage(const FString& MessageIdentifer, const FText& MessageBody, EAlertType AlertType);
	void AddMessageReceiver(const FString& RouteId, const TArray<FString>& Codes, FOnRoutedMessageReceived Delegate);
	void RemoveMessageReceiver(const FString& RouteId);
	void RouteHTTPResult(const FHttpResponsePtr& Res, const FString& FunctionName);
	void RouteFailureToConnect(const FString& FunctionName);

private:
	TArray<FErrorRoute> ErrorRoutes;
};
