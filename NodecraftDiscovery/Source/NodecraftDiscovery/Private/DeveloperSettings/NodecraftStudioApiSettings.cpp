// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include <DeveloperSettings/NodecraftStudioApiSettings.h>

FString UNodecraftStudioApiSettings::GetApiRoot() const
{
	return ApiRoot.TrimStartAndEnd();
}

FString UNodecraftStudioApiSettings::GetPublicGameToken() const
{
	FString Token = PublicGameToken.TrimStartAndEnd();
	ensureAlwaysMsgf(Token.Len() > 0, TEXT("You have an empty public game token. This must be set in the project settings."));
	return Token;
}

float UNodecraftStudioApiSettings::GetNotificationPollingInterval() const
{
	return NotificationPollingInterval;
}

float UNodecraftStudioApiSettings::GetServerJoinPollingInterval() const
{
	return ServerJoinPollingInterval;
}

float UNodecraftStudioApiSettings::GetFriendsPollingIntervalInGame() const
{
	return FriendsPollingIntervalInGame;
}

float UNodecraftStudioApiSettings::GetFriendsPollingIntervalOutOfGame() const
{
	return FriendsPollingIntervalOutOfGame;
}

float UNodecraftStudioApiSettings::GetServerHeartbeatRate() const
{
	return ServerHeartbeatRate;
}

FName UNodecraftStudioApiSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
