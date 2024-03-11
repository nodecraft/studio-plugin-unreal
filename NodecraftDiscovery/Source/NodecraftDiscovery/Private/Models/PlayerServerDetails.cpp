// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/PlayerServerDetails.h"

UPlayerDataObject* UPlayerServerDetailsDataObject::GetPlayer() const
{
	return Player;
}

UBanDataObject* UPlayerServerDetailsDataObject::GetBan() const
{
	return Ban;
}

UPlayerServerDetailsDataObject* UPlayerServerDetailsDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UPlayerServerDetailsDataObject* PlayerServerDetailsDataObject = NewObject<UPlayerServerDetailsDataObject>();
	PlayerServerDetailsDataObject->Player = UPlayerDataObject::FromJson(Json);
	if (const TSharedPtr<FJsonObject>* BanObject; Json->TryGetObjectField("ban", BanObject))
	{
		PlayerServerDetailsDataObject->Ban = UBanDataObject::FromJson(BanObject->ToSharedRef());
	}
	
	return PlayerServerDetailsDataObject;
}

UPlayerDataObject* UPlayerServerDetailsViewModel::GetPlayer() const
{
	return PlayerServerDetailsDataObject->GetPlayer();
}

UBanDataObject* UPlayerServerDetailsViewModel::GetBan() const
{
	return PlayerServerDetailsDataObject->GetBan();
}

UPlayerServerDetailsDataObject* UPlayerServerDetailsViewModel::GetPlayerServerDetails() const
{
	return PlayerServerDetailsDataObject;
}

UPlayerServerDetailsViewModel* UPlayerServerDetailsViewModel::FromDataObject(
	UPlayerServerDetailsDataObject* DataObject)
{
	UPlayerServerDetailsViewModel* PlayerServerDetailsViewModel = NewObject<UPlayerServerDetailsViewModel>();
	PlayerServerDetailsViewModel->PlayerServerDetailsDataObject = DataObject;
	return PlayerServerDetailsViewModel;	
}
