#pragma once

UENUM()
enum class EIdentityType : uint8
{
	Steam,
	Xbox,
	PSN,
	Nintendo,
	Android,
	Apple,
	SMS,
	Email,
	UNDEFINED,
	COUNT UMETA(Hidden)
};

namespace EIdentityTypeHelper
{
	static EIdentityType FromString(const FString& IdentTypeString)
	{
		if (IdentTypeString == "steam")
		{
			return EIdentityType::Steam;
		}
		else if (IdentTypeString == "xbox")
		{
			return EIdentityType::Xbox;
		}
		else if (IdentTypeString == "psn")
		{
			return EIdentityType::PSN;
		}
		else if (IdentTypeString == "nintendo")
		{
			return EIdentityType::Nintendo;
		}
		else if (IdentTypeString == "android")
		{
			return EIdentityType::Android;
		}
		else if (IdentTypeString == "apple")
		{
			return EIdentityType::Apple;
		}
		else if (IdentTypeString == "sms")
		{
			return EIdentityType::SMS;
		}
		else if (IdentTypeString == "email")
		{
			return EIdentityType::Email;
		}
		else
		{
			return EIdentityType::COUNT;
		}
	}
}
