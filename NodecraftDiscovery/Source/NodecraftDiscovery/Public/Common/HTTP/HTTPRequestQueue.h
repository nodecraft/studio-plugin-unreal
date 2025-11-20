// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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

	// "Cancelled" requests will still go through, but their delegates will not be called
	// and we will not call the FinishDelegate when they are done
	void CancelRequeusts();
};
