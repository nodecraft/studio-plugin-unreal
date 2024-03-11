// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/TabbedSections/ServerDetailsAboutSection.h"

#include "CommonListView.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "Components/WrapBox.h"
#include "UI/SocialLinks/SocialLinksList.h"
#include "Models/SocialLinkDataObject.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "ServerDetailsAboutSection"
DEFINE_LOG_CATEGORY_STATIC(LogServerAboutSection, All, All);

void UServerDetailsAboutSection::SetServerId(const FString& InServerId)
{
	ServerId = InServerId;
}

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

	if (ServerTagButtonClass.IsNull() == false)
	{
		ServerTags->ClearChildren();
		for (FString Tag : ServerDataObject->GetTags())
		{
			UClass* ButtonClass = ServerTagButtonClass.LoadSynchronous();
			UNodecraftButtonBase* TagWidget = CreateWidget<UNodecraftButtonBase>(this, ButtonClass);
			TagWidget->SetButtonText(FText::FromString(Tag));
			ServerTags->AddChild(TagWidget);
		}
		ServerTagsTitle->SetVisibility(ServerTags->GetChildrenCount() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	
	LoadGuard->SetIsLoading(false);
}

#undef LOCTEXT_NAMESPACE
