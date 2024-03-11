// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/HTTP/HTTPRequestQueue.h"
#include "Interfaces/IHttpResponse.h"

void FHTTPRequestQueue::RunQueue(UObject* WeakLambdaContext, TArray<TSharedRef<IHttpRequest>> Requests, FAllRequestsFinishedDelegate InFinishDelegate)
{
	Queue = Requests;
	FinishDelegate = InFinishDelegate;
	for (TSharedRef<IHttpRequest> RequestToRun : Requests)
	{
		// Inject our own lambda into the OnProcessRequestComplete delegate, which will call the original delegate and then our own RequestCompleted function
		auto InitialCompletedDelegate = RequestToRun.Get().OnProcessRequestComplete();
		RequestToRun.Get().OnProcessRequestComplete().BindWeakLambda(WeakLambdaContext, [this, RequestToRun, InitialCompletedDelegate](FHttpRequestPtr Req, FHttpResponsePtr Response, bool bConnectedSuccessfully)
		{
			InitialCompletedDelegate.ExecuteIfBound(Req, Response, bConnectedSuccessfully);
			RequestCompleted(Req, Response, bConnectedSuccessfully, RequestToRun);
		});

		RequestToRun.Get().ProcessRequest();
	}
}

void FHTTPRequestQueue::RequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully,
	TSharedRef<IHttpRequest> InRequest)
{
	Queue.Remove(InRequest);
	if (bConnectedSuccessfully == false || EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()) == false)
	{
		bAllCallsSucceeded = false;
	}
	if (Queue.IsEmpty())
	{
		FinishDelegate.ExecuteIfBound(bAllCallsSucceeded);
	}
}
