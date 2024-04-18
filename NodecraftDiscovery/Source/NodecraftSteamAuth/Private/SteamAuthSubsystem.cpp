// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "SteamAuthSubsystem.h"

#include "SteamAuthDeveloperSettings.h"
#include "ThirdParty/SteamAuthLibrary/sdk/public/steam/steam_api.h"

DEFINE_LOG_CATEGORY_STATIC(LogSteamAuth, Log, All);

void USteamAuthSubsystem::GetIdentTokenAsync(FOnIdentTokenReceived OnTokenReceived)
{
	// Cache this for later use since we can't pass it to the Steam API callback
	TokenReceivedCallback = OnTokenReceived;
	
	if (const USteamAuthDeveloperSettings* SteamSettings = GetDefault<USteamAuthDeveloperSettings>(); SteamSettings->GetAppId().IsEmpty())
	{
		ensureAlwaysMsgf(false, TEXT("Steam App ID is not set in the project settings but steam auth is enabled. Please set the Steam App ID in the project settings or disable Steam Auth."));
		UE_LOG(LogSteamAuth, Error, TEXT("Steam App ID is not set in the project settings"));
		OnTokenReceived.ExecuteIfBound(false, FString(), TEXT("Steam App ID is not set in the project settings"));
	}
	else
	{
		// Initialize Steam
#if !UE_BUILD_SHIPPING
		FFileHelper::SaveStringToFile(SteamSettings->GetAppId(), TEXT("steam_appid.txt"));
#endif

		SteamAPI_RestartAppIfNecessary(atoi(TCHAR_TO_ANSI(*SteamSettings->GetAppId())));
	
		bSteamInitialized = SteamAPI_Init();
		if (bSteamInitialized)
		{
			UE_LOG(LogSteamAuth, Log, TEXT("SteamAPI_Init() succeeded"));
			// TODO: Either remove this or replace it with an identity that the backend is also using
			const char* identity = "test";
			SteamUser()->GetAuthTicketForWebApi(identity);
			// NOTE: Once the ticket is received, OnReceivedAuthTicket will be called
		}
		else
		{
			UE_LOG(LogSteamAuth, Error, TEXT("SteamAPI_Init() failed"));
			OnTokenReceived.ExecuteIfBound(false, FString(), TEXT("SteamAPI_Init() failed"));
		}
	}
}

void USteamAuthSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USteamAuthSubsystem::Tick(float DeltaTime)
{
	SteamAPI_RunCallbacks();
}

// TODO: Move this into a Nodecraft Common module
FString CharPtrToHexString(const char* CharPtr, int32 Length)
{
	static constexpr char table[] = "0123456789ABCDEF";
	char* dst = new char[2 * Length + 1];
	const char* srcPtr = CharPtr;
	char* dstPtr = &dst[0];

	for (auto count = Length; count > 0; --count)
	{
		unsigned char c = *srcPtr++;
		// first digit (left-most) of two-digit hex value, so shift. This is a similar operation to multiplying by 16 (or multiplying by 10 in base-10)
		*dstPtr++ = table[c >> 4];
		// second digit of two-digit hex value, so mask. This is a similar operation to taking the remainder of a division by 16 (or 10 in base-10)
		*dstPtr++ = table[c & 0x0f];
	}
	*dstPtr = 0;
	return FString(dst);
}

void USteamAuthSubsystem::OnReceivedAuthTicket(GetTicketForWebApiResponse_t* pCallback )
{
	if (pCallback->m_eResult != k_EResultOK)
	{
		UE_LOG(LogSteamAuth, Error, TEXT("GetAuthTicketForWebApi failed with result %d"), pCallback->m_eResult);
		TokenReceivedCallback.ExecuteIfBound(false, FString(), FString::Printf(TEXT("GetAuthTicketForWebApi failed with result %d"), pCallback->m_eResult));
	}
	else
	{
		const auto ticket_ptr = reinterpret_cast<const char*>(pCallback->m_rgubTicket);
		const auto ticket_len = pCallback->m_cubTicket;

		const FString TokenHexString = CharPtrToHexString(ticket_ptr, ticket_len);
	
		TokenReceivedCallback.ExecuteIfBound(true, TokenHexString, TEXT(""));
	}
}
