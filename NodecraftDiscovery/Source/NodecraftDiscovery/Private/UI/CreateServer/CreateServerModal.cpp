// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/CreateServer/CreateServerModal.h"

#include "CommonInputSubsystem.h"
#include "Components/Image.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Common/NodecraftRadioButton.h"
#include "UI/Common/NodecraftScrollBox.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"

void UCreateServerModal::Configure(FSimpleDelegate OnModalClosed)
{
	CloseButton->OnClicked().Clear();
	CancelButton->OnClicked().Clear();
	
	CloseButton->OnClicked().AddWeakLambda(this, [OnModalClosed]
	{
		OnModalClosed.ExecuteIfBound();
	});
	CancelButton->OnClicked().AddWeakLambda(this, [OnModalClosed]
	{
		OnModalClosed.ExecuteIfBound();
	});
}

void UCreateServerModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetPersonalServerButton->OnClicked().AddWeakLambda(this, [this]
	{
		SelectPersonalServer();
		UMenuManagerSubsystem::Get().ShowInternalRedirectModal(EPlayerConnectionSubject::CreateEssentials);
	});
	GetNodecraftProButton->OnClicked().AddWeakLambda(this, [this]
	{
		SelectNodecraftProServer();
		UMenuManagerSubsystem::Get().ShowInternalRedirectModal(EPlayerConnectionSubject::CreatePro);
	});
	
	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UCreateServerModal::NativeConstruct()
{
	Super::NativeConstruct();
	
	SelectPersonalServer();

	PersonalServerButton->OnClicked().AddWeakLambda(this, [this]
	{
		SelectPersonalServer();
	});
	
	NodecraftProServerButton->OnClicked().AddWeakLambda(this, [this]
	{
		SelectNodecraftProServer();
	});

	NextButton->OnClicked().AddWeakLambda(this, [this]
	{
		if (NodecraftProRadioButton->GetIsChecked())
		{
			UMenuManagerSubsystem::Get().ShowInternalRedirectModal(EPlayerConnectionSubject::CreatePro);
		}
		else if (PersonalServerRadioButton->GetIsChecked())
		{
			UMenuManagerSubsystem::Get().ShowInternalRedirectModal(EPlayerConnectionSubject::CreateEssentials);
		}
	});

	PersonalServerButton->OnFocusReceived().AddWeakLambda(this, [this]
	{
		SelectPersonalServer();
	});
	NodecraftProServerButton->OnFocusReceived().AddWeakLambda(this, [this]
	{
		SelectNodecraftProServer();
	});
}

void UCreateServerModal::NativeDestruct()
{
	PersonalServerButton->OnClicked().Clear();
	NodecraftProServerButton->OnClicked().Clear();
	NextButton->OnClicked().Clear();
	Super::NativeDestruct();
}

void UCreateServerModal::NativeOnActivated()
{
	Super::NativeOnActivated();
	ScrollBox->SetListeningForInput(true);
}

void UCreateServerModal::NativeOnDeactivated()
{
	ScrollBox->SetListeningForInput(false);
	Super::NativeOnDeactivated();
}

UWidget* UCreateServerModal::NativeGetDesiredFocusTarget() const
{
	if (PersonalServerButton)
	{
		return PersonalServerButton;
	}
	return Super::NativeGetDesiredFocusTarget();
}

bool UCreateServerModal::NativeOnHandleBackAction()
{
	CloseButton->OnClicked().Broadcast();
	return Super::NativeOnHandleBackAction();
}

void UCreateServerModal::SelectPersonalServer()
{
	PersonalServerRadioButton->SetIsChecked(true);
	PersonalServerImage->SetBrushTintColor(SelectedImageTint);
	
	NodecraftProRadioButton->SetIsChecked(false);
	NodecraftProImage->SetBrushTintColor(UnselectedImageTint);
}

void UCreateServerModal::SelectNodecraftProServer()
{
	NodecraftProRadioButton->SetIsChecked(true);
    NodecraftProImage->SetBrushTintColor(SelectedImageTint);

	PersonalServerRadioButton->SetIsChecked(false);
	PersonalServerImage->SetBrushTintColor(UnselectedImageTint);
}

void UCreateServerModal::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	if (CurrentInputType == ECommonInputType::Gamepad)
	{
		if (NextActionData.IsNull() == false)
		{
			FBindUIActionArgs Args = FBindUIActionArgs(NextActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
				{
					NextButton->OnClicked().Broadcast();
				}));
			if (NextButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				Args.OverrideDisplayName = NextButton->GetInputActionNameOverride();
			}
			NextActionHandle = RegisterUIActionBinding(Args);
		}
	}
	else
	{
		if (NextActionHandle.IsValid())
		{
			NextActionHandle.Unregister();
		}
	}
}
