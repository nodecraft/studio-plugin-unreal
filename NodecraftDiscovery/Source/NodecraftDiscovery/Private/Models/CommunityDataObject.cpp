// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/CommunityDataObject.h"

FString UCommunityDataObject::GetId() const
{
	return Id;
}

FText UCommunityDataObject::GetTitle() const
{
	return Title;
}

FText UCommunityDataObject::GetSummary() const
{
	return Summary;
}

FString UCommunityDataObject::GetImageUrl() const
{
	return ImageUrl;
}

UCommunityDataObject* UCommunityDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UCommunityDataObject* CommunityDataObject = NewObject<UCommunityDataObject>();

	CommunityDataObject->Id = Json->GetStringField("id");
	CommunityDataObject->Title = FText::FromString(Json->GetStringField("title"));

	FString SummaryString;
	if (Json->TryGetStringField("summary", SummaryString))
	{
		CommunityDataObject->Summary = FText::FromString(SummaryString);
	}
	
	Json->TryGetStringField("image_avatar", CommunityDataObject->ImageUrl);

	return CommunityDataObject;
}
