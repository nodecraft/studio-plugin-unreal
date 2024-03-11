// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/JoiningServerQueueModal.h"

#include "CommonTextBlock.h"
#include "Components/ProgressBar.h"
#include "Models/ServerQueueDataObject.h"
#include "Models/ServerQueueTokenDataObject.h"
#include "Models/ServerSessionDataObject.h"
#include "Services/ServerQueueService.h"
#include "Services/ServersService.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "JoiningServerQueueModal"

void UJoiningServerQueueModal::Configure(const UServerQueueDataObject* ServerQueueDataObject, const UServerQueueTokenDataObject* InServerQueueTokenDataObject, FSimpleDelegate& OnPopupClosed)
{
	ServerQueueTokenDataObject = InServerQueueTokenDataObject;
	OnClosed = OnPopupClosed;

	UServerQueueService::Get(GetWorld())->OnGetServerSession.Clear();
	UServerQueueService::Get(GetWorld())->OnGetServerSession.AddDynamic(this, &UJoiningServerQueueModal::OnGetServerSession);

	UServerQueueService* QueueService = UServerQueueService::Get(GetWorld());
	QueueService->OnGetServerQueue.BindWeakLambda(this, [this, OnPopupClosed](UServerQueueDataObject* ServerQueue, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			WaitTimeInSeconds = ServerQueue->GetEstimatedWait();
			UpdateWaitTimeUI(WaitTimeInSeconds);
			if (ServerQueue->GetServerSession())
			{
				OnPopupClosed.ExecuteIfBound();
			}
		}
	});
	QueueService->OnRenewServerQueue.BindWeakLambda(this, [this, QueueService](UServerQueueTokenDataObject* ServerQueueToken)
	{
		ServerQueueTokenDataObject = ServerQueueToken;
		QueueService->StopPollingServerQueue();
		QueueService->StartPollingServerQueue(ServerQueueToken);
	});
	QueueService->StartPollingServerQueue(ServerQueueTokenDataObject);
	
	QueuePositionText->SetText(FText::Format(LOCTEXT("QueuePositionXofY", "position {0} of {1}"),
		ServerQueueDataObject->GetQueuePosition(), ServerQueueDataObject->GetQueueSize()));

	const float ProgressBarPercent = float(ServerQueueDataObject->GetQueuePosition()) / float(ServerQueueDataObject->GetQueueSize());
	ProgressBar->SetPercent(ProgressBarPercent);

	WaitTimeInSeconds = ServerQueueDataObject->GetEstimatedWait();
	UpdateWaitTimeUI(WaitTimeInSeconds);

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UJoiningServerQueueModal::CountdownTimerTick, 5.f, true);
	}
	
	CancelButton->OnClicked().AddWeakLambda(this, [this, OnPopupClosed]
	{
		FSimpleServiceResponseDelegate OnCancelServerQueue;
		OnCancelServerQueue.BindWeakLambda(this, [this, OnPopupClosed](bool bSuccess,  TOptional<FText> Error)
		{
			if (bSuccess)
			{
				if (UServerQueueService* QueueService = UServerQueueService::Get(GetWorld()))
				{
					QueueService->StopPollingServerQueue();
				}
				OnPopupClosed.ExecuteIfBound();
			}
		});
		UServerQueueService::Get(GetWorld())->CancelServerQueue(ServerQueueTokenDataObject->GetToken(), OnCancelServerQueue);
	});
}

void UJoiningServerQueueModal::CountdownTimerTick()
{
	if (WaitTimeInSeconds <= 0)
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(CountdownTimerHandle);
		}
	}
	else
	{
		WaitTimeInSeconds -= 5.f;
	}
	UpdateWaitTimeUI(WaitTimeInSeconds);
}

void UJoiningServerQueueModal::OnGetServerSession(const UServerSessionDataObject* Session)
{
	if (Session)
	{
		OnClosed.ExecuteIfBound();
	}
}

void UJoiningServerQueueModal::UpdateWaitTimeUI(float TimeLeftSeconds)
{
	if (TimeLeftSeconds == 0)
	{
		EstimatedWaitTime->SetText(LOCTEXT("EstimatedWaitTimeSoon", "Estimated wait time: Soon"));
	}
	else
	{
		EstimatedWaitTime->SetText(FText::Format(LOCTEXT("EstimatedWaitTime", "Estimated wait time: {0}"), WaitTimeInSeconds));
	}
}

#undef LOCTEXT_NAMESPACE
