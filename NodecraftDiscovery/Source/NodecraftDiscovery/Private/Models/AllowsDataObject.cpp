// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/AllowsDataObject.h"

#include "Models/AllowStatus.h"
#include "Models/ServerDataObject.h"

UAllowsDataObject* UAllowsDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UAllowsDataObject* AllowsDataObject = NewObject<UAllowsDataObject>();

	AllowsDataObject->ID = Json->GetStringField(TEXT("id"));

	const FString Status = Json->GetStringField(TEXT("status"));
	if (Status.Equals("pending", ESearchCase::IgnoreCase))
	{
		AllowsDataObject->AllowStatus = EAllowStatus::Pending;
	}
	else if (Status.Equals("accepted", ESearchCase::IgnoreCase))
	{
		AllowsDataObject->AllowStatus = EAllowStatus::Accepted;
	}
	else if (Status.Equals("declined", ESearchCase::IgnoreCase))
	{
		AllowsDataObject->AllowStatus = EAllowStatus::Declined;
	}
	else if (Status.Equals("revoked", ESearchCase::IgnoreCase))
	{
		AllowsDataObject->AllowStatus = EAllowStatus::Revoked;
	}

	FString DateString;
	if (Json->TryGetStringField(TEXT("date_expires"), DateString))
	{
		FDateTime::ParseIso8601(*DateString, AllowsDataObject->DateExpires);
	}
	else
	{
		AllowsDataObject->DateExpires = FDateTime();
	}
	
	AllowsDataObject->Server = UServerDataObject::FromJson(Json->GetObjectField(TEXT("server")).ToSharedRef());
	AllowsDataObject->Sender = UPlayerDataObject::FromJson(Json->GetObjectField(TEXT("sender")).ToSharedRef());

	const TSharedPtr<FJsonObject>* OutReceiver;
	if (Json->TryGetObjectField(TEXT("player"), OutReceiver))
	{
		AllowsDataObject->Receiver = UPlayerDataObject::FromJson(OutReceiver->ToSharedRef());
	}

	return AllowsDataObject;
}

FString UAllowsDataObject::GetID() const
{
	return ID;
}

EAllowStatus UAllowsDataObject::GetStatus() const
{
	return AllowStatus;
}

FDateTime UAllowsDataObject::GetDateExpires() const
{
	return DateExpires;
}

UPlayerDataObject* UAllowsDataObject::GetSender() const
{
	return Sender;
}

UPlayerDataObject* UAllowsDataObject::GetReceiver() const
{
	return Receiver;
}

UServerDataObject* UAllowsDataObject::GetServer() const
{
	return Server;
}

void UAllowsDataObject::SetStatus(const EAllowStatus Status)
{
	AllowStatus = Status;
}
