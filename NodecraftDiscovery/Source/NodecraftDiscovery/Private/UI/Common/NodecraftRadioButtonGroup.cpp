// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftRadioButtonGroup.h"

#include "UI/Foundation/NodecraftDynamicEntryBox.h"

UUserWidget* UNodecraftRadioButtonGroup::AddChildRadioButton(FNodecraftRadioButtonConfiguration& RadioButtonDescriptor, bool bIsChecked)
{
	int32 Index = Container->GetAllEntries().Num();
	CurrentSelectionIndex = bIsChecked ? Index : CurrentSelectionIndex;
	UUserWidget* Child = Container->CreateEntry();
	if (UNodecraftRadioButton* RadioButton = CastChecked<UNodecraftRadioButton>(Child))
	{
		RadioButton->Configure(RadioButtonDescriptor, Index);
		RadioButton->SetTextStyles(PrimaryTextStyle, SecondaryTextStyle, TertiaryTextStyle);
		RadioButton->SetIsChecked(bIsChecked);
		RadioButton->SetBorderStyles(ButtonBorderStyleDefault, ButtonBorderStyleFocused);
		RadioButton->SetBorderPadding(ButtonBorderPaddingDefault, ButtonBorderPaddingFocused);
		if (TriggeringActionNameOverride.IsEmpty() == false)
		{
			RadioButton->SetTriggeringInputAction(ButtonTriggeringInputAction, TriggeringActionNameOverride);
		}
		else
		{
			RadioButton->SetTriggeringInputAction(ButtonTriggeringInputAction);
		}
		RadioButton->OnRadioButtonSelected.AddUniqueDynamic(this, &UNodecraftRadioButtonGroup::HandleRadioButtonSelected);
		RadioButtonDescriptor.CreatedRadioButton = RadioButton;
		
		FNodecraftRadioButtonConfiguration* Descriptor =
			PreregisteredRadioButtons.FindByPredicate([RadioButtonDescriptor](const FNodecraftRadioButtonConfiguration& Entry)
			{
				return Entry.Id == RadioButtonDescriptor.Id;
			});
		// Descriptor wasn't in the preregistered list. This most likely means that the radio button was added at runtime.
		if (Descriptor == nullptr)
		{
			PreregisteredRadioButtons.Add(RadioButtonDescriptor);
		}
		OnAddedChildRadioButton.Broadcast(RadioButton);
	}
	return Child;
}

void UNodecraftRadioButtonGroup::HandleRadioButtonSelected(int32 ButtonIndex)
{
	if (ButtonIndex == CurrentSelectionIndex)
	{
		return;
	}
	
	if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(Container->GetEntryAtIndex(CurrentSelectionIndex)))
	{
		RadioButton->SetIsChecked(false);
	}
	
	CurrentSelectionIndex = ButtonIndex;
	if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(Container->GetEntryAtIndex(CurrentSelectionIndex)))
	{
		RadioButton->SetIsChecked(true);
	}
	
	OnRadioButtonSelected.Broadcast(ButtonIndex);
}

void UNodecraftRadioButtonGroup::SetSelectedIndex(int32 Index)
{
	HandleRadioButtonSelected(Index);
}

void UNodecraftRadioButtonGroup::SetSelectedId(const FString& Id)
{
	PreregisteredRadioButtons.FindByPredicate([Id, this](const FNodecraftRadioButtonConfiguration& Entry)
	{
		if (Entry.Id.ToString() == Id)
		{
			SetSelectedIndex(Entry.CreatedRadioButton->GetButtonIndex());
			return true;
		}
		return false;
	});
}

FString UNodecraftRadioButtonGroup::GetSelectedId() const
{
	if (PreregisteredRadioButtons.IsValidIndex(CurrentSelectionIndex))
	{
		return PreregisteredRadioButtons[CurrentSelectionIndex].Id.ToString();
	}
	return FString();
}

UUserWidget* UNodecraftRadioButtonGroup::GetSelectedEntry() const
{
	return Container->GetEntryAtIndex(CurrentSelectionIndex);
}

TArray<UUserWidget*> UNodecraftRadioButtonGroup::GetAllChildren() const
{
	return Container->GetAllEntries();
}

UUserWidget* UNodecraftRadioButtonGroup::GetChildAt(int32 Index)
{
	return Container->GetEntryAtIndex(Index);
}

UUserWidget* UNodecraftRadioButtonGroup::GetChildById(const FString& Id)
{
	FNodecraftRadioButtonConfiguration* ChildConfiguration = PreregisteredRadioButtons.FindByPredicate(
		[Id, this](const FNodecraftRadioButtonConfiguration& Entry)
		{
			if (Entry.Id.ToString() == Id)
			{
				return true;
			}
			return false;
		});
	if (ChildConfiguration)
	{
		return ChildConfiguration->CreatedRadioButton;
	}
	return nullptr;
}

int32 UNodecraftRadioButtonGroup::GetChildrenCount() const
{
	return Container->GetNumEntries();
}

bool UNodecraftRadioButtonGroup::HasAnyChildren() const
{
	return Container->GetAllEntries().Num() > 0;
}

void UNodecraftRadioButtonGroup::SynchronizeProperties()
{
	Super::SynchronizeProperties();	

	if (RadioButtonClass && IsEntryClassValid(RadioButtonClass))
	{
		Container->SetEntryWidgetClass(RadioButtonClass);
	}
	
	// At design time, preview the desired number of entries
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		if (RadioButtonClass == nullptr || IsEntryClassValid(RadioButtonClass) == false)
		{
			Container->Reset(true);
		}
		else if (Container->GetNumEntries() != NumDesignerPreviewEntries)
		{
			Container->Reset();
			int32 StartingNumber = Container->GetNumEntries();
			while (StartingNumber < NumDesignerPreviewEntries)
			{
				UUserWidget* Entry = Container->CreateEntryForDesigner(RadioButtonClass);
				if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(Entry))
				{
					RadioButton->SetTextStyles(PrimaryTextStyle, SecondaryTextStyle, TertiaryTextStyle);
					RadioButton->SetBorderStyles(ButtonBorderStyleDefault, ButtonBorderStyleFocused);
					RadioButton->SetBorderPadding(ButtonBorderPaddingDefault, ButtonBorderPaddingFocused);
				}
				StartingNumber++;
			}
		}
	}
#endif
	
	Container->SetEntrySpacing(EntrySpacing);
}

void UNodecraftRadioButtonGroup::NativeConstruct()
{
	Super::NativeConstruct();
	
	Container->Reset(true);
	for (FNodecraftRadioButtonConfiguration& RadioButtonInfo : PreregisteredRadioButtons)
	{
		AddChildRadioButton(RadioButtonInfo);
	}
}


void UNodecraftRadioButtonGroup::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OnAddedChildRadioButton.AddDynamic(this, &ThisClass::OnEntryWidgetGenerated);
}

FReply UNodecraftRadioButtonGroup::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (HasAnyChildren())
	{
		GetChildAt(0)->SetFocus();
		return FReply::Handled();
	}
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UNodecraftRadioButtonGroup::OnEntryWidgetGenerated(UNodecraftRadioButton* EntryWidget)
{
	if (EntryWidget)
	{
		EntryWidget->OnNavigation.BindWeakLambda(this, [this, EntryWidget]
			(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)->FNavigationReply
		{
			const UUserWidget* First = GetChildAt(0);
			const UUserWidget* Last = GetChildAt(GetChildrenCount() - 1);
			if (InNavigationEvent.GetNavigationType() == EUINavigation::Up && EntryWidget == First)
			{
				return FNavigationReply::Stop();
			}
			if (InNavigationEvent.GetNavigationType() == EUINavigation::Down && EntryWidget == Last)
			{
				return FNavigationReply::Stop();
			}
			if (InNavigationEvent.GetNavigationType() == EUINavigation::Left || InNavigationEvent.GetNavigationType() == EUINavigation::Right
				|| InNavigationEvent.GetNavigationType() == EUINavigation::Next || InNavigationEvent.GetNavigationType() == EUINavigation::Previous)
			{
				return FNavigationReply::Stop();
			}
			return InDefaultReply;
		});
	}
}

bool UNodecraftRadioButtonGroup::IsEntryClassValid(TSubclassOf<UUserWidget> EntryClass) const
{
	return EntryClass && EntryClass->IsChildOf(UNodecraftRadioButton::StaticClass());
}
