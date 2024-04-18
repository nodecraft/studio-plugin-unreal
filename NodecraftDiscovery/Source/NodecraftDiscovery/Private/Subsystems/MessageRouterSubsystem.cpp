// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "Subsystems/MessageRouterSubsystem.h"

#include "NodecraftLogCategories.h"
#include "Interfaces/IHttpResponse.h"
#include "Utility/NodecraftUtility.h"

static auto SendMsgClosure = [](const TArray<FString>& Args)
	{
		if (Args.Num() >= 3)
		{
			const FString MessageType = Args[0];
			const FString MessageCode = Args[1];
			EAlertType AlertType = EAlertType::Message;
			if (MessageType == "error")
			{
				AlertType = EAlertType::Error;
			}
			else if (MessageType == "success")
			{
				AlertType = EAlertType::Success;
			}
			// Include the command name in the message text
			// If you don't want to include the command name, use Args.GetData() + 2 instead of Args.GetData() + 1
			const TArray<FString> MessageTextArgs = TArray<FString>(Args.GetData() + 1, Args.Num() - 1);
			const FString MessageText = FString::Join(MessageTextArgs, TEXT(" "));

			UMessageRouterSubsystem::Get().RouteMessage(MessageCode, FText::FromString(MessageText), AlertType);
		}
		else
		{
			UE_LOG(LogNodecraftMessageRouter, Warning, TEXT("Need at least two strings for message code and message text."));
		}
	};


static FAutoConsoleCommand ConsoleCommandSendError(
	TEXT("NC.MessageRouter.SendError"),
	TEXT("Arguments: [0]:messageCode [1..n]:message text")
	TEXT("Send a message via the message router subsystem\n"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		TArray<FString> MsgArgs = Args;
		MsgArgs.Insert("error", 0);
		SendMsgClosure(MsgArgs);
	}),
	ECVF_Cheat);

static FAutoConsoleCommand ConsoleCommandSendMessage(
	TEXT("NC.MessageRouter.SendMessage"),
	TEXT("Arguments: [0]:messageCode [1..n]:message text")
	TEXT("Send a message via the message router subsystem\n"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		TArray<FString> MsgArgs = Args;
		MsgArgs.Insert("message", 0);
		SendMsgClosure(MsgArgs);
	}),
	ECVF_Cheat);

static FAutoConsoleCommand ConsoleCommandSendSuccess(
	TEXT("NC.MessageRouter.SendSuccess"),
	TEXT("Arguments: [0]:messageCode [1..n]:message text")
	TEXT("Send a success message via the message router subsystem\n"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		TArray<FString> MsgArgs = Args;
		MsgArgs.Insert("success", 0);
		SendMsgClosure(MsgArgs);
	}),
	ECVF_Cheat);

void UMessageRouterSubsystem::RouteMessage(const FString& MessageIdentifer, const FText& MessageBody,
                                           EAlertType AlertType)
{
	UE_LOG(LogNodecraftMessageRouter, VeryVerbose, TEXT("Routing message %s with body %s"), *MessageIdentifer, *MessageBody.ToString());
	for (FErrorRoute ErrorRoute : ErrorRoutes)
	{
		// match the route codes with the error identifier. The codes are either error identifiers or regex patterns
		for (FString Code : ErrorRoute.Codes)
		{
			// If the code matches perfectly or is a regex pattern that matches the error identifier
			if (FRegexMatcher(FRegexPattern(Code), MessageIdentifer).FindNext())
			{
				// call the delegate
				ErrorRoute.Delegate.ExecuteIfBound(MessageIdentifer, MessageBody, AlertType);
			}
		}
	}
}

void UMessageRouterSubsystem::AddMessageReceiver(const FString& RouteId, const TArray<FString>& Codes, FOnRoutedMessageReceived Delegate)
{
	UE_LOG(LogNodecraftMessageRouter, Verbose, TEXT("Adding message receiver for route %s"), *RouteId);
	// create route and add it to our routes
	const FErrorRoute Route = FErrorRoute(RouteId, Codes, Delegate);
	ErrorRoutes.Add(Route);
}

void UMessageRouterSubsystem::RemoveMessageReceiver(const FString& RouteId)
{
	UE_LOG(LogNodecraftMessageRouter, Verbose, TEXT("Removing message receiver for route %s"), *RouteId);
	// remove route from our routes
	ErrorRoutes.RemoveAll([RouteId](const FErrorRoute& Route)
	{
		return Route.Id == RouteId;
	});
}

void UMessageRouterSubsystem::RouteHTTPResult(const FHttpResponsePtr& Res, const FString& FunctionName)
{
	const int32 ResultCode = Res.Get()->GetResponseCode();
	EAlertType AlertType = EAlertType::Message;
	
	if (ResultCode >= 400)
	{
		AlertType = EAlertType::Error;
	}
	else if (EHttpResponseCodes::IsOk(ResultCode))
	{
		AlertType = EAlertType::Success;
	}
	
	const FString MessageResultCode = UNodecraftUtility::ParseMessageResultCode(Res);
	const FText Message = UNodecraftUtility::ParseMessage(Res, FunctionName);
	RouteMessage(MessageResultCode, Message, AlertType);
	// log message
}

void UMessageRouterSubsystem::RouteFailureToConnect(const FString& FunctionName)
{
#if UE_BUILD_SHIPPING
	FString ErrorMsg = "Error occurred while trying to connect";
#else
	FString ErrorMsg = FString::Printf(TEXT("%ls: Error occurred while trying to connect"), *FunctionName);
#endif
	RouteMessage("general.connection.failure", FText::FromString(ErrorMsg), EAlertType::Error);
}
