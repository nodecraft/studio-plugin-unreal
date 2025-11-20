// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/MainMenu/SearchField.h"

#include "Components/EditableTextBox.h"
#include "Services/ServersService.h"
#include "Stores/SearchStore.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void USearchField::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ClearSearchButton->OnClicked().AddWeakLambda(this, [this]
	{
		USearchStore::Get().ClearSearch();
	});

	SearchTextBox->OnTextChanged.AddUniqueDynamic(this, &USearchField::OnSearchTextChanged);
	SearchTextBox->OnTextCommitted.AddUniqueDynamic(this, &USearchField::OnSearchTextCommited);

	OnRequestClearSearch = USearchStore::Get().AddRequestClearSearchListener(
		FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(this, [this]
	{
		SearchTextBox->SetText(FText::GetEmpty());
		LastSearchText = SearchTextBox->GetText();
	}));
}

void USearchField::NativeDestruct()
{
	Super::NativeDestruct();
	USearchStore::Get().RemoveRequestClearSearchListener(OnRequestClearSearch);
}

FReply USearchField::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	SearchTextBox->SetFocus();
	return FReply::Handled();
}

void USearchField::OnSearchTextChanged(const FText& Text)
{
	if (Text.EqualTo(LastSearchText))
	{
		return;
	}
	
	LastSearchText = Text;
	UServersService::Get().SearchServers(GetWorld(), Text.ToString());
}

void USearchField::OnSearchTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnSearchTextChanged(Text);
}
