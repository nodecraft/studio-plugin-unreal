// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerCard.h"

#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Services/ServersService.h"
#include "Subsystems/MenuManagerSubsystem.h"

void UServerCard::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ServerDataObject = Cast<UServerDataObject>(ListItemObject);

	// If the server data object is empty, we are in the private server list and we should show the create server button
	if (ServerDataObject->GetId().IsEmpty())
	{
		Card->SetVisibility(ESlateVisibility::Collapsed);
		CreateServerButton->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	Card->SetVisibility(ESlateVisibility::Visible);
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
	JoinButton->OnClicked().AddWeakLambda(this, [this]
	{
		UServersService::Get().JoinServer(ServerDataObject, GetWorld());
	});
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