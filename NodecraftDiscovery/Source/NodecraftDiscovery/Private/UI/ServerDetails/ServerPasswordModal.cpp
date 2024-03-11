// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ServerPasswordModal.h"

#include "Components/EditableTextBox.h"
#include "DeveloperSettings/DiscoveryWidgetSettings.h"
#include "Services/ServerQueueService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UServerPasswordModal::Configure(const UServerDataObject* ServerDataObject, FSimpleDelegate InOnClosed)
{
	OnClosed = InOnClosed;
	UServerQueueService::Get(GetWorld())->OnGetServerSession.AddDynamic(this, &UServerPasswordModal::OnGetServerSession);
	
	CancelButton->OnClicked().AddWeakLambda(this, [InOnClosed]()
	{
		InOnClosed.ExecuteIfBound();
	});

	JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
	{
		UMenuManagerSubsystem::Get().ShowJoiningServerQueue(GetWorld(), ServerDataObject, InputBox->GetText().ToString());

		UMenuManagerSubsystem::Get().OnServerQueueError.BindWeakLambda(this, [this](const FText& ErrorText)
		{
			AlertMessage->Show(ErrorText, EAlertType::Error);
		});
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

void UServerPasswordModal::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

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

