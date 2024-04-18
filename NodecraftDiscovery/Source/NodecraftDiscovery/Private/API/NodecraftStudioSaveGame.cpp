// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "API/NodecraftStudioSaveGame.h"

FString UNodecraftStudioSaveGame::GetGameAccessToken() const
{
	return GameAccessToken;
}

void UNodecraftStudioSaveGame::SetGameAccessToken(const FString& InGameAccessToken)
{
	this->GameAccessToken = InGameAccessToken;
}

FString UNodecraftStudioSaveGame::GetPlayerAccessToken() const
{
	return PlayerAccessToken;
}

void UNodecraftStudioSaveGame::SetPlayerAccessToken(const FString& InPlayerAccessToken)
{
	this->PlayerAccessToken = InPlayerAccessToken;
}

FPlayerSession UNodecraftStudioSaveGame::GetPlayerSession() const
{
	return PlayerSession;
}

void UNodecraftStudioSaveGame::SetPlayerSession(const FPlayerSession& InPlayerSession)
{
	this->PlayerSession = InPlayerSession;
}
