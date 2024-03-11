// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerLists/ServerCard.h"

#include "Services/ServersService.h"
#include "Subsystems/MenuManagerSubsystem.h"

void UServerCard::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	ServerDataObject = Cast<UServerDataObject>(ListItemObject);
	// Populate the server card with the data from the json
	ServerNameTextBlock->SetText(ServerDataObject->GetTitle());

	ServerDescriptionTextBlock->SetText(ServerDataObject->GetSummary());

	// Populate the player count label by combining the players_count and players_max json fields
	const FString PlayerCount = FString::FromInt(ServerDataObject->GetPlayersCount());
	const FString PlayerMax = FString::FromInt(ServerDataObject->GetPlayersMax());
	ServerPlayersTextBlock->SetText(FText::FromString(PlayerCount + "/" + PlayerMax));

	if (ServerDataObject->GetImageUrl().IsEmpty() == false)
	{
		ServerImage->LoadImageAsync(ServerDataObject->GetImageUrl(), ServerDataObject->GetImageFilenameForCache());
	}

	FavoriteTag->SetVisibility(ServerDataObject->IsFavorite() ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	PrivateTag->SetVisibility(ServerDataObject->GetType() == EServerType::Private ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

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
		DEFINE_LOG_CATEGORY_STATIC(LogServerCard, All, All);
		UE_LOG(LogServerCard, Error, TEXT("ServerDataObject is null!"));
	}
}