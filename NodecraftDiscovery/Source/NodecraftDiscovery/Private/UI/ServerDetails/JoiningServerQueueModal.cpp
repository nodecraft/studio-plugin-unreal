// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/JoiningServerQueueModal.h"

#include "CommonTextBlock.h"
#include "Components/ProgressBar.h"
#include "Models/ServerQueueDataObject.h"
#include "Models/ServerSessionDataObject.h"
#include "Services/ServerQueueService.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "JoiningServerQueueModal"

void UJoiningServerQueueModal::Configure(FSimpleDelegate& OnPopupClosed)
{
	ClosePopupDelegate = OnPopupClosed;
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UJoiningServerQueueModal::CountdownTimerTick, 5.f, true);
	}
	
	CancelButton->OnClicked().AddWeakLambda(this, [this, OnPopupClosed]
	{
		if (UWorld* World = GetWorld())
		{
			UServerQueueService::Get(World)->CancelServerQueue();
		}
		OnPopupClosed.ExecuteIfBound();
	});
}

void UJoiningServerQueueModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UServerQueueService::Get(GetWorld())->OnGetServerSession.AddDynamic(this, &UJoiningServerQueueModal::OnGetServerSession);

	UServerQueueService* QueueService = UServerQueueService::Get(GetWorld());

	QueueService->AddGetServerQueueDelegate(FGetServerQueueDelegate::CreateWeakLambda(this, [this](UServerQueueDataObject* ServerQueue, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			if (ServerQueue)
			{
				const float ProgressBarPercent = float(ServerQueue->GetQueuePosition()) / float(ServerQueue->GetQueueSize());
				ProgressBar->SetPercent(ProgressBarPercent);
				QueuePositionText->SetText(FText::Format(LOCTEXT("QueuePositionXofY", "position {0} of {1}"),
				ServerQueue->GetQueuePosition(), ServerQueue->GetQueueSize()));
				WaitTimeInSeconds = ServerQueue->GetEstimatedWait();
				UpdateWaitTimeUI(WaitTimeInSeconds);
				if (ServerQueue->GetServerSession())
				{
					EstimatedWaitTime->SetText(FText::GetEmpty());
					QueuePositionText->SetText(LOCTEXT("JoiningServer", "Joining server..."));
				}
			}
		}
	}));

	QueuePositionText->SetText(LOCTEXT("WaitingToJoinQueue", "Waiting to join queue..."));
	EstimatedWaitTime->SetText(FText::GetEmpty());
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
		ClosePopupDelegate.ExecuteIfBound();
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
