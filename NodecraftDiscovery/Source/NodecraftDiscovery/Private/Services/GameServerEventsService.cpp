// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/GameServerEventsService.h"

#include "JsonObjectWrapper.h"
#include "API/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

#define LOCTEXT_NAMESPACE "GameServerEventsService"

DEFINE_LOG_CATEGORY_STATIC(LogGameServerEventsService, Log, All);

bool UGameServerEventsService::SendHeartbeat(const FHeartbeatRequestDataObject& HeartbeatRequestDataObject)
{
	// Log that we're calling send heartbeat, and include the request data object
	UE_LOG(LogGameServerEventsService, Log, TEXT("Sending heartbeat: %s"), *HeartbeatRequestDataObject.ToString());	
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		// Log response
		UE_LOG(LogGameServerEventsService, Log, TEXT("Heartbeat response received"));
		FHeartbeatResponseDataObject HeartbeatResponseData = FHeartbeatResponseDataObject();
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			FJsonObjectWrapper ResJson;
			ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
			if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
			{
				// Log success along with payload
				UE_LOG(LogGameServerEventsService, Log, TEXT("Heartbeat response data: %s"), *UNodecraftUtility::JsonObjToString(ResJson.JsonObject));
				HeartbeatResponseData = FHeartbeatResponseDataObject::FromJson(Data.ToSharedRef());
				OnHeartbeatResponse.Broadcast(HeartbeatResponseData, true, FText());
			}
			else
			{
				// Log failure with payload
				UE_LOG(LogGameServerEventsService, Error, TEXT("Failed to parse heartbeat response data: %s"), *UNodecraftUtility::JsonObjToString(ResJson.JsonObject));
				OnHeartbeatResponse.Broadcast(HeartbeatResponseData, false, FText::FromString("Failed to parse heartbeat response data"));
			}
		}
		else
		{
			// Log failure
			UE_LOG(LogGameServerEventsService, Error, TEXT("Failed to send heartbeat: %s"), *UNodecraftUtility::ParseMessage(Response, __FUNCTION__).ToString());
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
			OnHeartbeatResponse.Broadcast(HeartbeatResponseData, false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::SendHeartbeat(this, HeartbeatRequestDataObject, ReqCallback)->ProcessRequest();
}

bool UGameServerEventsService::SetServerStatus(bool bIsOnline, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully) {
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
			OnComplete.ExecuteIfBound(false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::SetServerStatus(this, bIsOnline, ReqCallback)->ProcessRequest();
}

bool UGameServerEventsService::ValidatePlayerOnJoin(const FPlayerJoinRequestData& PlayerJoinReqData,
	FPlayerJoinResponseDelegate OnComplete)
{
	// Log that we're validating player on join
	UE_LOG(LogGameServerEventsService, Log, TEXT("Validating player on join"));
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		// Log response
		UE_LOG(LogGameServerEventsService, Log, TEXT("Validate player on join response received"));
		FPlayerJoinResponseDataObject PlayerJoinResponseData = FPlayerJoinResponseDataObject();
		if (bConnectedSuccessfully) {
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			FJsonObjectWrapper ResJson;
			ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
			if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
			{
				// Log success along with payload
				UE_LOG(LogGameServerEventsService, Log, TEXT("Validate player on join response data: %s"), *UNodecraftUtility::JsonObjToString(ResJson.JsonObject));
				PlayerJoinResponseData = FPlayerJoinResponseDataObject::FromJson(Data.ToSharedRef());
				OnComplete.ExecuteIfBound(PlayerJoinResponseData, true, FText());
			}
		}
		else
		{
			// Log failure
			UE_LOG(LogGameServerEventsService, Error, TEXT("Failed to validate player on join: %s"), *UNodecraftUtility::ParseMessage(Response, __FUNCTION__).ToString());
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
			OnComplete.ExecuteIfBound(PlayerJoinResponseData, false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::ValidatePlayerOnJoin(this, PlayerJoinReqData, ReqCallback)->ProcessRequest();
}

bool UGameServerEventsService::TrackPlayerLeft(const FPlayerLeaveRequestData& PlayerLeaveReqData,
	FSimpleServiceResponseDelegate OnComplete)
{
	// Log with request data
	UE_LOG(LogGameServerEventsService, Log, TEXT("Tracking player left: %s"), *PlayerLeaveReqData.ToString());
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully) {
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			// Log success
			UE_LOG(LogGameServerEventsService, Log, TEXT("Player left tracked successfully"));
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			// Log failure
			UE_LOG(LogGameServerEventsService, Error, TEXT("Failed to track player left: %s"), *UNodecraftUtility::ParseMessage(Response, __FUNCTION__).ToString());
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
			OnComplete.ExecuteIfBound(false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::TrackPlayerLeft(this, PlayerLeaveReqData, ReqCallback)->ProcessRequest();
}

bool UGameServerEventsService::TrackPlayerKicked(const FString& SessionId, const FString& KickId,
	FSimpleServiceResponseDelegate OnComplete)
{
	// Log
	UE_LOG(LogGameServerEventsService, Log, TEXT("Tracking player kicked"));
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			// Log success
			UE_LOG(LogGameServerEventsService, Log, TEXT("Player kicked tracked successfully"));
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			// Log failure
			UE_LOG(LogGameServerEventsService, Error, TEXT("Failed to track player kicked: %s"), *UNodecraftUtility::ParseMessage(Response, __FUNCTION__).ToString());
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
			OnComplete.ExecuteIfBound(false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::TrackPlayerKick(this, SessionId, KickId, ReqCallback)->ProcessRequest();
}

bool UGameServerEventsService::FinishedReconnectionPhase()
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				// Log success
				UE_LOG(LogGameServerEventsService, Verbose, TEXT("Call to Finished Reconnection Phase successful"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			// Log failure
			UE_LOG(LogGameServerEventsService, Error, TEXT("Failed call to Finished Reconnection Phase: %s"), *UNodecraftUtility::ParseMessage(Response, __FUNCTION__).ToString());
			const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
		}
	});
	return UNodecraftStudioApi::FinishedReconnectionPhase(this, ReqCallback)->ProcessRequest();
}

#undef LOCTEXT_NAMESPACE
