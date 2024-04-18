// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ModerationReasonDataObject.h"

FString UModerationReasonDataObject::GetId() const
{
	return Id;
}

FText UModerationReasonDataObject::GetTitle() const
{
	return Title;
}

FText UModerationReasonDataObject::GetDescription() const
{
	return Description;
}

UModerationReasonDataObject* UModerationReasonDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UModerationReasonDataObject* ModerationReasonDataObject = NewObject<UModerationReasonDataObject>();

	ModerationReasonDataObject->Id = Json->GetStringField("id");
	ModerationReasonDataObject->Title = FText::FromString(Json->GetStringField("title"));
	ModerationReasonDataObject->Description = FText::FromString(Json->GetStringField("description"));

	return ModerationReasonDataObject;
}
