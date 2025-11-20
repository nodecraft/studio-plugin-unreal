// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/Consents.h"

FConsentPageDetails FConsentPageDetails::FromJson(const TSharedPtr<FJsonObject>& Json)
{
	FConsentPageDetails Details = FConsentPageDetails();
	Json.Get()->TryGetStringField(TEXT("page_id"), Details.PageId);
	Json.Get()->TryGetStringField(TEXT("page_content_id"), Details.PageContentId);
	Json.Get()->TryGetStringField(TEXT("entity"), Details.Entity);
	Json.Get()->TryGetStringField(TEXT("type"), Details.Type);
	Json.Get()->TryGetStringField(TEXT("entity_id"), Details.EntityId);
	if (Details.EntityId.Equals("null", ESearchCase::IgnoreCase))
	{
		Details.EntityId = "";
	}

	return Details;
}

TSharedRef<FJsonObject> FConsentPageDetails::ToJson() const
{
	FJsonObject Obj = FJsonObject();
	Obj.SetStringField("page_id", PageId);
	Obj.SetStringField("page_content_id", PageContentId);
	Obj.SetStringField("type", Type);
	return MakeShared<FJsonObject>(Obj);
}

bool FConsentPageDetails::operator==(const FConsentPageDetails& Element) const
{
	return PageId.Equals(Element.PageId, ESearchCase::CaseSensitive) &&
		PageContentId.Equals(Element.PageContentId, ESearchCase::CaseSensitive) &&
		Type.Equals(Element.Type, ESearchCase::CaseSensitive) &&
		Entity.Equals(Element.Entity, ESearchCase::CaseSensitive) &&
		EntityId.Equals(Element.EntityId, ESearchCase::CaseSensitive);
}

FString FGameConsents::GetConsentType() const
{
	return ConsentPages.IsEmpty() ? "" : ConsentPages[0].Type;
}

bool FGameConsents::operator==(const FGameConsents& GameConsents) const
{
	if (ConsentPages.Num() != GameConsents.ConsentPages.Num())
	{
		return false;
	}
	for (int i = 0; i < ConsentPages.Num(); ++i)
	{
		if (ConsentPages[i] == GameConsents.ConsentPages[i] == false)
		{
			return false;
		}
	}
	if (Content.Equals(GameConsents.Content, ESearchCase::CaseSensitive) == false)
	{
		return false;
	}
	return true;
}

bool FGameConsents::operator!=(const FGameConsents& GameConsents) const
{
	return !(*this == GameConsents);
}

TArray<TSharedPtr<FJsonValue>> FGameConsents::ToJsonArray() const
{
	FJsonObject Obj = FJsonObject();

	TArray<TSharedPtr<FJsonValue>> ConsentsArray;

	for (const FConsentPageDetails& Page : ConsentPages)
	{
		ConsentsArray.Add(MakeShared<FJsonValueObject>(FJsonValueObject(Page.ToJson())));
	}
	
	return ConsentsArray;
}

FGameConsents FGameConsents::FromDataJson(const TSharedPtr<FJsonObject>& Json)
{
	FGameConsents GameConsents = FGameConsents();
	
	const TArray<TSharedPtr<FJsonValue>>* ConsentPages;
	if (Json->TryGetArrayField(TEXT("consents"), ConsentPages))
	{
		if (Json->TryGetStringField(TEXT("content"), GameConsents.Content))
		{
			// GameConsents.Content = Content;
			for (TSharedPtr<FJsonValue> JsonValue : *ConsentPages)
			{
				FConsentPageDetails ConsentPage = FConsentPageDetails::FromJson(JsonValue.Get()->AsObject());
				GameConsents.ConsentPages.Add(ConsentPage);
			}
		}
	}
	return GameConsents;
}

FGameConsents FGameConsents::FromDataJsonArray(const TArray<TSharedPtr<FJsonValue>>& Json)
{
	FGameConsents GameConsents = FGameConsents();
	
	for (TSharedPtr<FJsonValue> JsonValue : Json)
	{
		FConsentPageDetails ConsentPage = FConsentPageDetails::FromJson(JsonValue.Get()->AsObject());
		GameConsents.ConsentPages.Add(ConsentPage);
	}
	return GameConsents;
}

FPlayerConsents FPlayerConsents::FromJson(const TSharedPtr<FJsonObject>& Json)
{
	FPlayerConsents PlayerConsents = FPlayerConsents();

	PlayerConsents.Content = Json->GetStringField(TEXT("content"));

	FString Status = Json->GetStringField(TEXT("status_all"));
	if (Status.Equals("missing", ESearchCase::IgnoreCase))
	{
		PlayerConsents.ConsentsStatusAll = EConsentsStatus::Missing;
	}
	else if (Status.Equals("outdated", ESearchCase::IgnoreCase))
	{
		PlayerConsents.ConsentsStatusAll = EConsentsStatus::Outdated;
	}
	else if (Status.Equals("signed", ESearchCase::IgnoreCase))
	{
		PlayerConsents.ConsentsStatusAll = EConsentsStatus::Signed;
	}
	
	if (FString DateString; Json.Get()->TryGetStringField(TEXT("date_all_signed"), DateString))
	{
		FDateTime::ParseIso8601(*DateString, PlayerConsents.DateAllSigned);
	}

	return PlayerConsents;
}
