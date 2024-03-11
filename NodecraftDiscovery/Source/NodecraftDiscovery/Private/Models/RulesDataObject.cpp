// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/RulesDataObject.h"

FText URulesDataObject::GetContent() const
{
	return FText::FromString(Content);
}

EConsentStatus URulesDataObject::GetConsentStatus() const
{
	EConsentStatus Status = EConsentStatus::Unknown;
	
	if (StatusRaw.Equals("missing", ESearchCase::IgnoreCase))
	{
		Status = EConsentStatus::NotSigned;
	}
	else if (StatusRaw.Equals("outdated", ESearchCase::IgnoreCase))
	{
		Status = EConsentStatus::Outdated;
	}
	else if (StatusRaw.Equals("signed", ESearchCase::IgnoreCase))
	{
		Status = EConsentStatus::Signed;
	}

	return Status;
}

FDateTime URulesDataObject::GetDateSigned() const
{
	FDateTime DateSigned;
	const bool bSuccess = FDateTime::ParseIso8601(*DateAllSigned, DateSigned);
	if (bSuccess == false)
	{
		DEFINE_LOG_CATEGORY_STATIC(LogRulesDataObject, All, All)
		UE_LOG(LogRulesDataObject, Error, TEXT("URulesDataObject::GetDateSigned(): Failed to parse date: %s"), *DateAllSigned);
	}
	return DateSigned;
}

void URulesDataObject::SetDateSigned(const FDateTime InSigned = FDateTime::UtcNow())
{
	DateAllSigned = InSigned.ToIso8601();
}

FGameConsents URulesDataObject::GetConsents() const
{
	return Consents;
}

void URulesDataObject::Update(const URulesDataObject* InRules)
{
	if (InRules->Consents != FGameConsents())
	{
		Consents = InRules->Consents;
	}
	if (InRules->Content.IsEmpty() == false)
	{
		Content = InRules->Content;
	}
	if (InRules->StatusRaw.IsEmpty() == false)
	{
		StatusRaw = InRules->StatusRaw;
	}
	if (InRules->DateAllSigned.IsEmpty() == false)
	{
		DateAllSigned = InRules->DateAllSigned;
	}
}

URulesDataObject* URulesDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	URulesDataObject* RulesDataObject = NewObject<URulesDataObject>();
	RulesDataObject->Content = Json->GetStringField("content");
	RulesDataObject->StatusRaw = Json->GetStringField("status_all");
	RulesDataObject->DateAllSigned = Json->GetStringField("date_all_signed");
	RulesDataObject->Consents = FGameConsents::FromDataJson(Json);
	
	return RulesDataObject;
}
