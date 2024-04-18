// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftRadioButton.h"

#include "Components/Image.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UNodecraftRadioButton::SetIsChecked(bool bIsChecked)
{
	Button->SetIsSelected(bIsChecked);
	ButtonStateBackground->SetVisibility(bIsChecked ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	ButtonStateImage->SetVisibility(bIsChecked ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	ButtonStateImage->SetColorAndOpacity(bIsChecked ? SelectedTint : FColor::White);
}

bool UNodecraftRadioButton::GetIsChecked() const
{
	return Button->GetSelected();
}

void UNodecraftRadioButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked().Clear();
	Button->OnClicked().AddWeakLambda(this, [this]()
	{
		SetIsChecked(true);
		OnNodecraftRadioButtonClicked.ExecuteIfBound();
	});
}
