// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
// #include "Templates/SharedPointer.h"

DECLARE_DELEGATE_OneParam(FAllRequestsFinishedDelegate, bool /* bSuccess */);

struct FHTTPRequestQueue : public TSharedFromThis<FHTTPRequestQueue>
{
	TArray<TSharedRef<IHttpRequest>> Queue;
	FAllRequestsFinishedDelegate FinishDelegate;
	bool bAllCallsSucceeded = true;

	virtual ~FHTTPRequestQueue()
	{
		FinishDelegate.Unbind();
		Queue.Empty();
	}
	
	void RunQueue(UObject* WeakLambdaContext, TArray<TSharedRef<IHttpRequest>> Requests, FAllRequestsFinishedDelegate InFinishDelegate);

	void RequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, TSharedRef<IHttpRequest> InRequest);
};
