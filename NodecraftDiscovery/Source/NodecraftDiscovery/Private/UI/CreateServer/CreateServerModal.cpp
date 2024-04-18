// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/CreateServer/CreateServerModal.h"

#include "Components/Image.h"
#include "DataTypes/LinkTypes.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Common/NodecraftRadioButton.h"
#include "UI/Foundation/NodecraftButtonBase.h"

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
}

void UCreateServerModal::NativeDestruct()
{
	PersonalServerButton->OnClicked().Clear();
	NodecraftProServerButton->OnClicked().Clear();
	NextButton->OnClicked().Clear();
	Super::NativeDestruct();
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
