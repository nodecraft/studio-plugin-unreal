// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerInvites/ServerInvitesListItem.h"

#include "CommonTextBlock.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/AsyncImage.h"

void UServerInvitesListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UServerDataObject* ServerDataObject = Cast<UServerDataObject>(ListItemObject);

	ServerImage->LoadImageAsync(ServerDataObject->GetImageUrl(), ServerDataObject->GetImageFilenameForCache());
	ServerTitle->SetText(ServerDataObject->GetTitle());
	ServerDescription->SetText(ServerDataObject->GetSummary());
	PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), ServerDataObject->GetPlayersCount(), ServerDataObject->GetPlayersMax())));
}
