// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerPasswordModal.h"

#include "Components/EditableTextBox.h"
#include "Services/ServerQueueService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "ServerPasswordModal"

void UServerPasswordModal::Configure(UServerDataObject* ServerDataObject, FSimpleDelegate InOnClosed)
{
	OnClosed = InOnClosed;
	UServerQueueService::Get(GetWorld())->OnGetServerSession.AddDynamic(this, &UServerPasswordModal::OnGetServerSession);
	CancelButton->OnClicked().Clear();
	CancelButton->OnClicked().AddWeakLambda(this, [InOnClosed, this]()
	{
		if (UWorld* World = GetWorld())
		{
			UServerQueueService::Get(World)->CancelServerQueue();
		}
		InOnClosed.ExecuteIfBound();
	});

	// Listen for a bad password
	UServerQueueService::Get(GetWorld())->OnPasswordIncorrect.AddWeakLambda(this, [this]()
	{
		AlertMessage->Show(LOCTEXT("IncorrectPassword", "Incorrect password"), EAlertType::Error);
	});

	JoinButton->OnClicked().Clear();
	JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
	{
		if (UWorld* World = GetWorld())
		{
			UServerQueueService::Get(World)->JoinServer(ServerDataObject,  InputBox->GetText().ToString());
		}
	});

	UMenuManagerSubsystem::Get().OnServerQueueError.BindWeakLambda(this, [this](const FText& ErrorText)
	{
		AlertMessage->Show(ErrorText, EAlertType::Error);
	});
}

void UServerPasswordModal::NativeConstruct()
{
	Super::NativeConstruct();

	ShowHidePasswordButton->OnClicked().AddWeakLambda(this, [this]()
	{
		InputBox->SetIsPassword(!InputBox->GetIsPassword());
	});
}

void UServerPasswordModal::NativeOnActivated()
{
	Super::NativeOnActivated();
	// Once we have a server queue we can close this modal
	TransitionToJoiningServerQueueHandle = UServerQueueService::Get(GetWorld())->OnStartedPollingServerQueue.AddWeakLambda(this, [this]()
	{
		OnClosed.ExecuteIfBound();
		UMenuManagerSubsystem::Get().ShowJoiningServerQueue();
	});
}

void UServerPasswordModal::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UServerQueueService::Get(GetWorld())->OnStartedPollingServerQueue.Remove(TransitionToJoiningServerQueueHandle);
	TransitionToJoiningServerQueueHandle.Reset();

	AlertMessage->Hide();
	InputBox->SetText(FText::GetEmpty());
	InputBox->SetIsPassword(true);
}

void UServerPasswordModal::OnGetServerSession(const UServerSessionDataObject* Session)
{
	if (Session)
	{
		OnClosed.ExecuteIfBound();
	}
}

#undef LOCTEXT_NAMESPACE