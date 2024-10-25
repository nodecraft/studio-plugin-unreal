// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsAboutSection.h"

#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Components/WrapBox.h"
#include "UI/SocialLinks/SocialLinksList.h"
#include "UI/Common/NodecraftLoadGuard.h"

#define LOCTEXT_NAMESPACE "ServerDetailsAboutSection"
DEFINE_LOG_CATEGORY_STATIC(LogServerAboutSection, All, All);

void UServerDetailsAboutSection::SetServerId(const FString& InServerId)
{
	ServerId = InServerId;
}

void UServerDetailsAboutSection::NativeConstruct()
{
	Super::NativeConstruct();

	SocialLinksList->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
	{
		EntryWidget.SetFocus();
		SocialLinksList->OnEntryWidgetGenerated().RemoveAll(this);
	});

	SocialLinksList->OnControllerMovedUpFromFirstItem.BindWeakLambda(this, [this]
	{
		ContentScrollBox->ScrollToStart();
	});

	SocialLinksList->OnControllerMovedDownFromLastItem.BindWeakLambda(this, [this]
	{
		ContentScrollBox->ScrollToEnd();
		if (ServerTags->HasAnyChildren())
		{
			ServerTags->GetChildAt(0)->SetFocus();
		}
	});
}

// UWidget* UServerDetailsAboutSection::NativeGetDesiredFocusTarget() const
// {
// 	if (SocialLinksList->GetFirstWidget())
// 	{
// 		return SocialLinksList->GetFirstWidget();
// 	}
// 	return Super::NativeGetDesiredFocusTarget();
// }

void UServerDetailsAboutSection::SetServerData(UServerDataObject* InServerDataObject)
{
	LoadGuard->SetIsLoading(true);
	
	ServerDataObject = InServerDataObject;
	if (InServerDataObject == nullptr)
	{
		LoadGuard->SetIsLoading(false);
		UE_LOG(LogServerAboutSection, Error, TEXT("UServerDetailsAboutSection::SetServerData(): ServerData is null!"));
		return;
	}

	SummaryText->SetText(ServerDataObject->GetSummary());
	
	ServerTypeCreatedByText->SetText(FText::Format(LOCTEXT("ServerCreatedBy","{0} is created by:"),
		ServerDataObject->GetType() == EServerType::Community ?
		LOCTEXT("CommunityServer", "Community Server") : LOCTEXT("PrivateServer", "Private Server")));

	ServerCreatedByText->SetText(ServerDataObject->GetTitle());
	
	SocialLinksList->SetListItems(ServerDataObject->GetSocialLinks());

	ServerTags->CreateTags(ServerDataObject);
	ServerTagsTitle->SetVisibility(ServerTags->GetChildrenCount() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	
	LoadGuard->SetIsLoading(false);
}

#undef LOCTEXT_NAMESPACE
