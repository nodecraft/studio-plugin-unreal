// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/BanDataObject.h"


FString UBanDataObject::GetId() const
{
	return Id;
}

EBanScope UBanDataObject::GetScope() const
{
	return Scope;
}

FDateTime UBanDataObject::GetDateExpires() const
{
	return DateExpires;
}

FDateTime UBanDataObject::GetDateCreated() const
{
	return DateCreated;
}

UPlayerDataObject* UBanDataObject::GetAuthor() const
{
	return Author;
}

FText UBanDataObject::GetPublicReason() const
{
	return PublicReason;
}

FText UBanDataObject::GetReason() const
{
	return Reason;
}

UBanDataObject* UBanDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UBanDataObject* BanDataObject = NewObject<UBanDataObject>();

	BanDataObject->Id = Json->GetStringField("id");

	const FString ScopeString = Json->GetStringField("scope");
	if (ScopeString == "server")
	{
		BanDataObject->Scope = EBanScope::Server;
	}
	else if (ScopeString == "community")
	{
		BanDataObject->Scope = EBanScope::Community;
	}
	else if (ScopeString == "game")
	{
		BanDataObject->Scope = EBanScope::Game;
	}
	else if (ScopeString == "global")
	{
		BanDataObject->Scope = EBanScope::Global;
	}

	FString DateExpiresString;
	Json->TryGetStringField("date_expires", DateExpiresString);
	FDateTime::ParseIso8601(*DateExpiresString, BanDataObject->DateExpires);

	
	FString DateCreatedString;
	Json->TryGetStringField("date_created", DateCreatedString);
	FDateTime::ParseIso8601(*DateCreatedString, BanDataObject->DateCreated);

	FString PublicReasonString;
	if (Json->TryGetStringField("public_reason", PublicReasonString))
	{
		BanDataObject->PublicReason = FText::FromString(PublicReasonString);
	}
	
	// reason
	if (FString ReasonStr; Json->TryGetStringField("reason", ReasonStr))
	{
		BanDataObject->Reason = FText::FromString(ReasonStr);
	}

	const TSharedPtr<FJsonObject>* AuthorObject;
	if (Json->TryGetObjectField("author", AuthorObject))
	{
		BanDataObject->Author = UPlayerDataObject::FromJson(AuthorObject->ToSharedRef());
	}
	
	return BanDataObject;
}
