// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerPasswordModal.h"

#include "CommonInputSubsystem.h"
#include "Components/EditableTextBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/ServerQueueService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"

#define LOCTEXT_NAMESPACE "ServerPasswordModal"

void UServerPasswordModal::Configure(UServerDataObject* InServerDataObject, FSimpleDelegate InOnClosed)
{
	OnClosed = InOnClosed;
	ServerDataObject = InServerDataObject;
	UServerQueueService::Get(GetWorld())->OnGetServerSession.AddUniqueDynamic(this, &UServerPasswordModal::OnGetServerSession);
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
	JoinButton->OnClicked().AddWeakLambda(this, [this, InServerDataObject]
	{
		if (const UWorld* World = GetWorld())
		{
			UServerQueueService::Get(World)->JoinServer(InServerDataObject,  InputBox->GetText().ToString());
		}
	});
	
	UMenuManagerSubsystem::Get().OnServerQueueError.BindWeakLambda(this, [this](const FText& ErrorText)
	{
		AlertMessage->Show(ErrorText, EAlertType::Error);
	});
	ON_INPUT_METHOD_CHANGED(RefreshActions);
}

void UServerPasswordModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InputBox->OnTextCommitted.AddUniqueDynamic(this, &UServerPasswordModal::OnTextCommitted);
	InputBox->OnTextChanged.AddUniqueDynamic(this, &UServerPasswordModal::OnTextChanged);
	ShowHidePasswordButton->OnClicked().AddWeakLambda(this, [this]()
	{
		ToggleShowPassword();
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
	JoinButton->SetIsEnabled(InputBox->GetText().IsEmpty() == false);
}

void UServerPasswordModal::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.RemoveAll(this);
	}
	UServerQueueService::Get(GetWorld())->OnStartedPollingServerQueue.Remove(TransitionToJoiningServerQueueHandle);
	TransitionToJoiningServerQueueHandle.Reset();

	AlertMessage->Hide();
	InputBox->SetText(FText::GetEmpty());
	InputBox->SetIsPassword(true);
	UnregisterActionBindings();
}

UWidget* UServerPasswordModal::NativeGetDesiredFocusTarget() const
{
	return InputBox ? InputBox : Super::NativeGetDesiredFocusTarget();
}

bool UServerPasswordModal::NativeOnHandleBackAction()
{
	CancelButton->OnClicked().Broadcast();
	return Super::NativeOnHandleBackAction();
}

void UServerPasswordModal::UnregisterActionBindings()
{
	if (JoinButtonActionHandle.IsValid())
	{
		JoinButtonActionHandle.Unregister();
	}
	if (PasswordVisibilityToggleButtonActionHandle.IsValid())
	{
		PasswordVisibilityToggleButtonActionHandle.Unregister();
	}
}

void UServerPasswordModal::RefreshActions(ECommonInputType InputType)
{
	if (InputType == ECommonInputType::Gamepad)
	{
		if (JoinButtonActionData.IsNull() == false && JoinButtonActionHandle.IsValid() == false && JoinButton->GetIsEnabled())
		{
			FBindUIActionArgs JoinActionArgs =
				FBindUIActionArgs(JoinButtonActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]
					{
						AttemptToJoinServer();
					}));
			if (JoinButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				JoinActionArgs.OverrideDisplayName = JoinButton->GetInputActionNameOverride();
			}
			JoinButtonActionHandle = RegisterUIActionBinding(JoinActionArgs);
		}

		if (PasswordVisibilityToggleButtonActionData.IsNull() == false && PasswordVisibilityToggleButtonActionHandle.IsValid() == false)
		{
			FBindUIActionArgs PasswordVisibilityToggleActionArgs =
				FBindUIActionArgs(PasswordVisibilityToggleButtonActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]
					{
						ToggleShowPassword();
					}));
			if (ShowHidePasswordButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				PasswordVisibilityToggleActionArgs.OverrideDisplayName = ShowHidePasswordButton->GetInputActionNameOverride();
			}
			PasswordVisibilityToggleButtonActionHandle = RegisterUIActionBinding(PasswordVisibilityToggleActionArgs);
		}
	}
	else
	{
		UnregisterActionBindings();
	}
}

void UServerPasswordModal::AttemptToJoinServer()
{
	if (JoinButton->GetIsEnabled() && ServerDataObject)
	{
		if (const UWorld* World = GetWorld())
		{
			UServerQueueService::Get(World)->JoinServer(ServerDataObject,  InputBox->GetText().ToString());
		}
	}
}

void UServerPasswordModal::ToggleShowPassword()
{
	InputBox->SetIsPassword(!InputBox->GetIsPassword());
}

void UServerPasswordModal::OnGetServerSession(const UServerSessionDataObject* Session)
{
	if (Session)
	{
		OnClosed.ExecuteIfBound();
	}
}

void UServerPasswordModal::OnTextChanged(const FText& InText)
{
	JoinButton->SetIsEnabled(InText.IsEmpty() == false);
}

void UServerPasswordModal::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		JoinButton->OnClicked().Broadcast();
	}
}

#undef LOCTEXT_NAMESPACE