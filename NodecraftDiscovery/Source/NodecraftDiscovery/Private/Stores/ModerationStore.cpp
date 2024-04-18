// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/ModerationStore.h"

FDelegateHandle UModerationStore::AddModeratorsChangedListener(const FOnModeratorsUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(Moderators);
	return OnModeratorsUpdated.Add(Delegate);
}

FDelegateHandle UModerationStore::AddOnlinePlayersChangedListener(const FOnOnlinePlayersUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(OnlinePlayers);
	return OnOnlinePlayersUpdated.Add(Delegate);
}

FDelegateHandle UModerationStore::AddOfflinePlayersChangedListener(const FOnOfflinePlayersUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(OfflinePlayers);
	return OnOfflinePlayersUpdated.Add(Delegate);
}

FDelegateHandle UModerationStore::AddBannedPlayersChangedListener(const FOnBannedPlayersUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(BannedPlayers);
	return OnBannedPlayersUpdated.Add(Delegate);
}

FDelegateHandle UModerationStore::AddOwnerChangedListener(const FOnOwnerUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(Owner);
	return OnOwnerUpdated.Add(Delegate);
}

FDelegateHandle UModerationStore::AddModerationReasonsUpdatedListener(
	const FSimpleMulticastDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound();
	return OnModerationReasonsUpdated.Add(Delegate);
}

bool UModerationStore::RemoveModeratorsChangedListener(const FDelegateHandle& Handle)
{
	return OnModeratorsUpdated.Remove(Handle);
}

bool UModerationStore::RemoveOnlinePlayersChangedListener(const FDelegateHandle& Handle)
{
	return OnOnlinePlayersUpdated.Remove(Handle);
}

bool UModerationStore::RemoveOfflinePlayersChangedListener(const FDelegateHandle& Handle)
{
	return OnOfflinePlayersUpdated.Remove(Handle);
}

bool UModerationStore::RemoveBannedPlayersChangedListener(const FDelegateHandle& Handle)
{
	return OnBannedPlayersUpdated.Remove(Handle);
}

bool UModerationStore::RemoveOwnerChangedListener(const FDelegateHandle& Handle)
{
	return OnOwnerUpdated.Remove(Handle);
}

void UModerationStore::OnModeratorDemoted(UPlayerServerDetailsDataObject* Player)
{
	Moderators.Remove(Player);
	OnModeratorsUpdated.Broadcast(Moderators);
	PlaceNonModeratorInCorrectList(Player);
}

void UModerationStore::OnModeratorPromoted(UPlayerServerDetailsDataObject* Player)
{
	Moderators.AddUnique(Player);
	OnModeratorsUpdated.Broadcast(Moderators);
	RemoveModeratorFromOtherLists(Player);
}

void UModerationStore::RemoveBannedPlayer(UPlayerServerDetailsDataObject* Player)
{
	Moderators.Remove(Player);
	OnModeratorsUpdated.Broadcast(Moderators);
	PlaceNonModeratorInCorrectList(Player);
}

void UModerationStore::PlaceNonModeratorInCorrectList(UPlayerServerDetailsDataObject* Player)
{
	if (Player->GetBan())
	{
		BannedPlayers.AddUnique(Player);
		OnBannedPlayersUpdated.Broadcast(BannedPlayers);
	}
	else if (Player->GetPlayer()->IsOnline())
	{
		OnlinePlayers.AddUnique(Player);
		OnOnlinePlayersUpdated.Broadcast(OnlinePlayers);
	}
	else
	{
		OfflinePlayers.AddUnique(Player);
		OnOfflinePlayersUpdated.Broadcast(OfflinePlayers);
	}
}

void UModerationStore::RemoveModeratorFromOtherLists(UPlayerServerDetailsDataObject* Player)
{
	if (BannedPlayers.Contains(Player))
	{
		BannedPlayers.Remove(Player);
		OnBannedPlayersUpdated.Broadcast(BannedPlayers);
	}
	else if (OnlinePlayers.Contains(Player))
	{
		OnlinePlayers.Remove(Player);
		OnOnlinePlayersUpdated.Broadcast(OnlinePlayers);
	}
	else if (OfflinePlayers.Contains(Player))
	{
		OfflinePlayers.Remove(Player);
		OnOfflinePlayersUpdated.Broadcast(OfflinePlayers);
	}
}

void UModerationStore::ClearAllPlayers()
{
	Moderators.Empty();
	OnlinePlayers.Empty();
	OfflinePlayers.Empty();
	BannedPlayers.Empty();
	Owner = nullptr;
	OnModeratorsUpdated.Broadcast(Moderators);
	OnOnlinePlayersUpdated.Broadcast(OnlinePlayers);
	OnOfflinePlayersUpdated.Broadcast(OfflinePlayers);
	OnBannedPlayersUpdated.Broadcast(BannedPlayers);
	OnOwnerUpdated.Broadcast(Owner);
}

void UModerationStore::SetModerators(const TArray<UPlayerServerDetailsDataObject*> Players)
{
	Moderators.Empty();
	for (UPlayerServerDetailsDataObject* Player : Players)
	{
		Moderators.AddUnique(Player);
	}
	OnModeratorsUpdated.Broadcast(Moderators);
}

void UModerationStore::SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*> Players)
{
	OnlinePlayers = Players;
	OnOnlinePlayersUpdated.Broadcast(OnlinePlayers);
}

void UModerationStore::SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*> Players)
{
	OfflinePlayers = Players;
	OnOfflinePlayersUpdated.Broadcast(OfflinePlayers);
}

void UModerationStore::SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*> Players)
{
	BannedPlayers = Players;
	OnBannedPlayersUpdated.Broadcast(BannedPlayers);
}

void UModerationStore::SetOwner(UPlayerServerDetailsDataObject* InOwner)
{
	Owner = InOwner;
	OnOwnerUpdated.Broadcast(InOwner);
}

bool UModerationStore::IsPlayerModerator(UPlayerServerDetailsDataObject* Player) const
{
	return Moderators.Contains(Player);
}

TArray<UModerationReasonDataObject*> UModerationStore::GetModerationReasons() const
{
	return ModerationReasons;
}

void UModerationStore::SetModerationReasons(TArray<UModerationReasonDataObject*> InReasons)
{
	ModerationReasons = InReasons;
	OnModerationReasonsUpdated.Broadcast();
}
