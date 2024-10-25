// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerQueueIdleCheckModal.h"

#include "Services/ServerQueueService.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UServerQueueIdleCheckModal::Configure(FSimpleDelegate& OnClose)
{
	BackButton->OnClicked().Clear();
	BackButton->OnClicked().AddWeakLambda(this, [this, OnClose]()
	{
		CancelQueueAndCloseModal(OnClose);
	});

	CloseButton->OnClicked().AddWeakLambda(this, [this, OnClose]
	{
		CancelQueueAndCloseModal(OnClose);
	});

	ContinueButton->OnClicked().AddWeakLambda(this, [this, OnClose]
	{
		UServerQueueService::Get(GetWorld())->RenewServerQueue();
		OnClose.ExecuteIfBound();
	});

	UServerQueueService::Get(GetWorld())->OnServerQueueCancelled.AddWeakLambda(this, [OnClose]
	{
		OnClose.ExecuteIfBound();
	});
}

void UServerQueueIdleCheckModal::CancelQueueAndCloseModal(const FSimpleDelegate& OnClose)
{
	UServerQueueService* ServerQueueService = UServerQueueService::Get(GetWorld());
	ServerQueueService->CancelServerQueue();
	OnClose.ExecuteIfBound();
}
