#pragma once

UENUM()
enum class EPlayerConnectionSubject
{
	Undefined,
	Support,
	Billing,
	CreateEssentials,
	CreatePro,
	ServerSettings
};

namespace EPlayerConnectionSubjectHelper
{
	static EPlayerConnectionSubject FromString(const FString& IdentTypeString)
	{
		if (IdentTypeString.Equals("support"))
		{
			return EPlayerConnectionSubject::Support;
		}
		if (IdentTypeString.Equals("billing"))
		{
			return EPlayerConnectionSubject::Billing;
		}
		if (IdentTypeString.Equals("create/essentials"))
		{
			return EPlayerConnectionSubject::CreateEssentials;
		}
		if (IdentTypeString.Equals("create/pro"))
		{
			return EPlayerConnectionSubject::CreatePro;
		}
		if (IdentTypeString.Contains("server/"))
		{
			return EPlayerConnectionSubject::ServerSettings;
		}
		return EPlayerConnectionSubject::Undefined;
	}

	static FString ToString(const EPlayerConnectionSubject PlayerConnectionSubject)
	{
		switch (PlayerConnectionSubject)
		{
		case EPlayerConnectionSubject::Undefined:
			return "";
		case EPlayerConnectionSubject::Support:
			return "support";
		case EPlayerConnectionSubject::Billing:
			return "billing";
		case EPlayerConnectionSubject::CreateEssentials:
			return "create/essentials";
		case EPlayerConnectionSubject::CreatePro:
			return "create/pro";
		case EPlayerConnectionSubject::ServerSettings:
			return "server";
		default:
			return "";
		}
	}
}