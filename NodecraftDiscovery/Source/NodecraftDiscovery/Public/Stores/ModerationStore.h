// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/ModerationReasonDataObject.h"
#include "Models/PlayerServerDetails.h"
#include "Subsystems/WorldSubsystem.h"
#include "ModerationStore.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOnlinePlayersUpdated, TArray<UPlayerServerDetailsDataObject*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOfflinePlayersUpdated, TArray<UPlayerServerDetailsDataObject*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnModeratorsUpdated, TArray<UPlayerServerDetailsDataObject*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBannedPlayersUpdated, TArray<UPlayerServerDetailsDataObject*>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOwnerUpdated, UPlayerServerDetailsDataObject*);


UCLASS()
class NODECRAFTDISCOVERY_API UModerationStore : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UModerationStore* Get(const UWorld* World)
	{
		if (World == nullptr)
		{
			return nullptr;
		}
		
		return World->GetSubsystem<UModerationStore>();
	}

	FDelegateHandle AddModeratorsChangedListener(const FOnModeratorsUpdated::FDelegate& Delegate);
	FDelegateHandle AddOnlinePlayersChangedListener(const FOnOnlinePlayersUpdated::FDelegate& Delegate);
	FDelegateHandle AddOfflinePlayersChangedListener(const FOnOfflinePlayersUpdated::FDelegate& Delegate);
	FDelegateHandle AddBannedPlayersChangedListener(const FOnBannedPlayersUpdated::FDelegate& Delegate);
	FDelegateHandle AddOwnerChangedListener(const FOnOwnerUpdated::FDelegate& Delegate);
	FDelegateHandle AddModerationReasonsUpdatedListener(const FSimpleMulticastDelegate::FDelegate& Delegate);

	bool RemoveModeratorsChangedListener(const FDelegateHandle& Handle);
	bool RemoveOnlinePlayersChangedListener(const FDelegateHandle& Handle);
	bool RemoveOfflinePlayersChangedListener(const FDelegateHandle& Handle);
	bool RemoveBannedPlayersChangedListener(const FDelegateHandle& Handle);
	bool RemoveOwnerChangedListener(const FDelegateHandle& Handle);
	void OnModeratorDemoted(UPlayerServerDetailsDataObject* Player);
	void OnModeratorPromoted(UPlayerServerDetailsDataObject* Player);
	void ClearAllPlayers();
	void SetModerators(const TArray<UPlayerServerDetailsDataObject*> Players);
	void SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*> Players);
	void SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*> Players);
	void SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*> Players);
	void SetOwner(UPlayerServerDetailsDataObject* InOwner);
	bool IsPlayerModerator(UPlayerServerDetailsDataObject* Player) const;

	TArray<UModerationReasonDataObject*> GetModerationReasons() const;
	void SetModerationReasons(TArray<UModerationReasonDataObject*> InReasons);

private:
	FOnModeratorsUpdated OnModeratorsUpdated;
	FOnOnlinePlayersUpdated OnOnlinePlayersUpdated;
	FOnOfflinePlayersUpdated OnOfflinePlayersUpdated;
	FOnBannedPlayersUpdated OnBannedPlayersUpdated;
	FOnOwnerUpdated OnOwnerUpdated;
	FSimpleMulticastDelegate OnModerationReasonsUpdated;
	
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> Moderators;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> OnlinePlayers;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> OfflinePlayers;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> BannedPlayers;

	UPROPERTY()
	UPlayerServerDetailsDataObject* Owner;

	UPROPERTY()
	TArray<UModerationReasonDataObject*> ModerationReasons;

	void RemoveBannedPlayer(UPlayerServerDetailsDataObject* Player);
	void PlaceNonModeratorInCorrectList(UPlayerServerDetailsDataObject* Player);
	void RemoveModeratorFromOtherLists(UPlayerServerDetailsDataObject* Player);
};
