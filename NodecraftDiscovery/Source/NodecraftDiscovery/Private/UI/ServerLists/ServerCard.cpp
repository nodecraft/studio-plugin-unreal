// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerCard.h"

#include "CommonInputSubsystem.h"
#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Services/ServersService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "Components/Border.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/ServerQueueService.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
#include "Utility/NodecraftMacros.h"

void UServerCard::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetServerData(Cast<UServerDataObject>(ListItemObject));
 }

void UServerCard::SetServerData(UServerDataObject* InServerData)
{
	ServerDataObject = Cast<UServerDataObject>(InServerData);

	if (InServerData == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	// If the server data object is empty, we are in the private server list and we should show the create server button
	if (ServerDataObject->GetId().IsEmpty())
	{
		Card->SetVisibility(ESlateVisibility::Collapsed);
		CreateServerButton->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	Card->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CreateServerButton->SetVisibility(ESlateVisibility::Collapsed);
	
	// Populate the server card with the data from the json
	ServerNameTextBlock->SetText(ServerDataObject->GetTitle());

	ServerDescriptionTextBlock->SetText(ServerDataObject->GetSummary());

	// Populate the player count label by combining the players_count and players_max json fields
	const FString PlayerCount = FString::FromInt(ServerDataObject->GetPlayersCount());
	const FString PlayerMax = FString::FromInt(ServerDataObject->GetPlayersMax());
	ServerPlayersTextBlock->SetText(FText::FromString(PlayerCount + " / " + PlayerMax));

	if (ServerDataObject->GetImageUrl().IsEmpty() == false)
	{
		ServerImage->LoadImageAsync(ServerDataObject->GetImageUrl(), ServerDataObject->GetImageFilenameForCache());
	}

	FavoriteTag->SetVisibility(ServerDataObject->IsFavorite() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	PrivateTag->SetVisibility(ServerDataObject->GetType() == EServerType::Private ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	if (const UPlayerDataObject* Owner = ServerDataObject->GetPlayer())
	{
		OwnerTag->SetVisibility(Owner->GetId() == UNodecraftStudioSessionManager::Get().GetPlayerId()
			? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	
	ResetJoinButtonLoadingState();
	UMenuManagerSubsystem& MenuManager = UMenuManagerSubsystem::Get();
	MenuManager.OnPushJoiningServerQueuePopupDelegate.AddWeakLambda(this, [this]
	{
		ResetJoinButtonLoadingState();
	});
	MenuManager.OnRequestShowServerDetailsModal.AddWeakLambda(
		this, [this](UServerDataObject* InServerDataObject, TOptional<FServerDetailsArgument> Args)
		{
			ResetJoinButtonLoadingState();
		});
	UServerQueueService::Get(GetWorld())->OnServerQueueCancelled.AddWeakLambda(this, [this]
	{
		ResetJoinButtonLoadingState();
	});
	JoinButton->OnClicked().Clear();
	JoinButton->OnClicked().AddWeakLambda(this, [this]
	{
		JoinButton->SetIsLoading(true);
		UServersService::Get().JoinServer(ServerDataObject, GetWorld());
	});
	ShowServerDetailsButton->OnClicked().Clear();
	ShowServerDetailsButton->OnClicked().AddWeakLambda(this, [this]
    {
        OpenServerDetails();
    });
}

FReply UServerCard::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (CreateServerButton->IsVisible() == false)
	{
		if (JoinServerInputActionData.IsNull() == false && JoinServerUIActionHandle.IsValid() == false)
		{
			FBindUIActionArgs UIActionArgs =
				FBindUIActionArgs(JoinServerInputActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]()
					{
						JoinButton->OnClicked().Broadcast();
					}));
			if (JoinButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				UIActionArgs.OverrideDisplayName = JoinButton->GetInputActionNameOverride();
			}
			JoinServerUIActionHandle = RegisterUIActionBinding(UIActionArgs);
		}

		if (ShowServerDetailsInputActionData.IsNull() == false && ShowServerDetailsUIActionHandle.IsValid() == false)
        {
            FBindUIActionArgs UIActionArgs =
                FBindUIActionArgs(ShowServerDetailsInputActionData, bDisplayInActionBar,
                    FSimpleDelegate::CreateWeakLambda(this, [this]()
                    {
                        ShowServerDetailsButton->OnClicked().Broadcast();
                    }));
			if (ShowServerDetailsButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				UIActionArgs.OverrideDisplayName = ShowServerDetailsButton->GetInputActionNameOverride();
			}
            ShowServerDetailsUIActionHandle = RegisterUIActionBinding(UIActionArgs);
        }
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UServerCard::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	if (JoinServerUIActionHandle.IsValid())
	{
		JoinServerUIActionHandle.Unregister();
	}
	if (ShowServerDetailsUIActionHandle.IsValid())
	{
		ShowServerDetailsUIActionHandle.Unregister();
	}
	Super::NativeOnFocusLost(InFocusEvent);
}

void UServerCard::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ON_INPUT_METHOD_CHANGED(RefreshActions);
}

void UServerCard::OpenServerDetails()
{
	// Create a server details modal
	if (ServerDataObject)
	{
		UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject);
	}
	else
	{
		UE_LOG(LogNodecraftMisc, Error, TEXT("ServerDataObject is null!"));
	}
}

void UServerCard::ResetBorderStyle()
{
	Border->SetBrushColor(BorderColorDefault);
	Border->SetPadding(BorderPaddingDefault);
}

void UServerCard::ResetJoinButtonLoadingState() const
{
	JoinButton->SetIsLoading(false);
}

void UServerCard::RefreshActions(ECommonInputType InputType)
{
    if (InputType != ECommonInputType::Gamepad)
    {
    	if (JoinServerUIActionHandle.IsValid())
    	{
    		JoinServerUIActionHandle.Unregister();
    	}
    	if (ShowServerDetailsUIActionHandle.IsValid())
    	{
    		ShowServerDetailsUIActionHandle.Unregister();
    	}
    	ResetBorderStyle();
    }
}
