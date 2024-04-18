// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerDetailsHeaderCard.h"

#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "Models/ServerDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Common/AsyncImage.h"

#define LOCTEXT_NAMESPACE "ServerDetailsHeaderCard"

void UServerDetailsHeaderCard::SetServerData(UServerDataObject* ServerDataObject, bool bIsAwaitingMoreCompleteData)
{
	if (ServerDataObject)
	{
		ServerImage->LoadImageAsync(ServerDataObject->GetImageUrl(), ServerDataObject->GetImageFilenameForCache());

		const FText ServerTypeText = ServerDataObject->GetType() == EServerType::Community ? LOCTEXT("CommunityServer", "Community Server") : LOCTEXT("PrivateServer", "Private Server");
		ServerTypeTextBlock->SetText(ServerTypeText);

		FServerTypeStyle Style = ServerTypeStyle.FindChecked(ServerDataObject->GetType());
		if (Style.BorderStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.BorderStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
			{
				ServerTypeBorder->SetStyle(Style.BorderStyle.Get());
			}));
		}
		if (Style.TextStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.TextStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
			{
				ServerTypeTextBlock->SetStyle(Style.TextStyle.Get());
			}));
		}

		ServerNameTextBlock->SetText(ServerDataObject->GetTitle());
		
		// server is created by a player OR a community. only 1 of them should be present at a time.
		if (const UCommunityDataObject* CommunityDataObject = ServerDataObject->GetCommunity(); CommunityDataObject != nullptr)
		{
			ServerOwnerInfoTextBlock->SetText(CommunityDataObject->GetTitle());
		}
		else if (const UPlayerDataObject* PlayerDataObject = ServerDataObject->GetPlayer())
		{
			const FText Username = FText::FromString(PlayerDataObject->GetUsername());
			ServerOwnerInfoTextBlock->SetText(FText::GetEmpty());
			if (Username.IsEmpty() == false)
			{
				ServerOwnerInfoTextBlock->SetText(Username);
			}
		}
		else
		{
			ServerOwnerInfoTextBlock->SetText(FText::GetEmpty());
		}
		ServerRegionTextBlock->SetText(ServerDataObject->GetRegionTitle());
		GameVersionTextBlock->SetText(ServerDataObject->GetGameVersion());

		TagsTextBlock->SetText(FText::GetEmpty());
		FString Tags = "";
		for (FString Tag : ServerDataObject->GetTags())
		{
			Tags.Append(Tag);
			Tags.Append(", ");
		}
		TagsTextBlock->SetText(FText::GetEmpty());
		if (Tags.IsEmpty() == false)
		{
			Tags = Tags.TrimEnd();
            Tags.RemoveFromEnd(",");
            TagsTextBlock->SetText(FText::FromString(Tags));
		}
	}
	LoadGuard->SetIsLoading(bIsAwaitingMoreCompleteData == true);
}

void UServerDetailsHeaderCard::NativeConstruct()
{
	Super::NativeConstruct();
	LoadGuard->SetIsLoading(true);
}

#undef LOCTEXT_NAMESPACE
