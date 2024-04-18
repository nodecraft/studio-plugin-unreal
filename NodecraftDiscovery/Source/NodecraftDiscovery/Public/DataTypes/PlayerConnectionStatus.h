// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

UENUM()
enum class EPlayerConnectionStatus : uint8
{
	Undefined,
	Pending,
	CodeChallenge,
	Completed,
	Expired,
};

namespace EPlayerConnectionStatusHelpers
{
	static EPlayerConnectionStatus FromString(const FString& StatusString)
	{
		if (StatusString.Equals("pending"))
		{
			return EPlayerConnectionStatus::Pending;
		}
		else if (StatusString.Equals("completed"))
		{
			return EPlayerConnectionStatus::Completed;
		}
		else if (StatusString.Equals("expired"))
		{
			return EPlayerConnectionStatus::Expired;
		}
		else if (StatusString.Equals("code_challenge"))
		{
			return EPlayerConnectionStatus::CodeChallenge;
		}
		else
		{
			return EPlayerConnectionStatus::Undefined;
		}
	}
	
	static FString ToString(EPlayerConnectionStatus Status)
	{
		switch (Status)
		{
		case EPlayerConnectionStatus::Undefined:
			return TEXT("undefined");
		case EPlayerConnectionStatus::Pending:
			return TEXT("pending");
		case EPlayerConnectionStatus::CodeChallenge:
			return TEXT("code_challenge");
		case EPlayerConnectionStatus::Completed:
			return TEXT("completed");
		case EPlayerConnectionStatus::Expired:
			return TEXT("expired");
		default:
			return TEXT("undefined");
		}
	}
}
