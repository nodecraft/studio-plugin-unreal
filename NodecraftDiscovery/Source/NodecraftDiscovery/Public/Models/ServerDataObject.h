// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerDataObject.h"
#include "RulesDataObject.h"
#include "UObject/Object.h"
#include "CommunityDataObject.h"
#include "ServerDataObject.generated.h"


class USocialLinkDataObject;

UENUM()
enum class EServerType : uint8
{
	Unknown = 0,
	Community = 1,
	Private = 2
};

UENUM()
enum class EPlayerRole : uint8
{
	Unknown = 0,
	Player = 1,
	Moderator = 2,
	Owner = 3
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerDataObject* FromJson(const TSharedRef<FJsonObject> Json);

	
	FString GetId() const;
	EServerType GetType() const;
	FText GetTitle() const;
	FText GetSummary() const;
	FString GetImageUrl() const;
	FString GetImageFilenameForCache() const;
	FText GetRegionTitle() const;
	FText GetGameVersion() const;
	int32 GetPlayersCount() const;
	int32 GetPlayersMax() const;
	bool IsFavorite() const;
	bool HasPassword() const;
	TArray<FString> GetTags() const;
	TArray<USocialLinkDataObject*> GetSocialLinks() const;
	UCommunityDataObject* GetCommunity() const;
	EPlayerRole GetRole() const;

	void SetRules(URulesDataObject* InRules);
	void SetIsFavorite(bool InIsFavorite);

	URulesDataObject* GetRules() const;
	UPlayerDataObject* GetPlayer() const;

	// Merges the fields from InRules with the current rules, giving precendence to InRules if it has a value
	void UpdateRules(URulesDataObject* InRules);
	
private:
	FString Id;
	EServerType Type;
	EPlayerRole Role;
	FString Title;
	FString Summary;
	FString ImageUrl;
	FText RegionTitle;
	FText GameVersion;
	int32 PlayersCount;
	int32 PlayersMax;
	bool bIsFavorite;
	bool bHasPassword;
	TArray<FString> Tags;

	// Number of friends currently playing on the server. Present when inviting to a server.
	int32 NumFriendsPlaying;

	// Number of friends who recently played on the server. Present when inviting to a server.
	int32 NumFriendsRecentlyPlayed;

	UPROPERTY()
	UCommunityDataObject* Community;
	
	UPROPERTY()
	URulesDataObject* Rules;

	UPROPERTY()
	UPlayerDataObject* PlayerDataObject;

	UPROPERTY()
	TArray<USocialLinkDataObject*> SocialLinks;
};
