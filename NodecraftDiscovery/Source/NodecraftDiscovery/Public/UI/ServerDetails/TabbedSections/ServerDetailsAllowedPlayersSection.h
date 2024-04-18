// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerDetailsSection.h"
#include "ServerDetailsAllowedPlayersSection.generated.h"

class UIconTextLoadingButton;
class UAllowsDataObject;
class UAlertMessage;
class UCommonTextBlock;
class UNodecraftLoadGuard;
class UCommonListView;
class UCommonBorder;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsAllowedPlayersSection : public UCommonActivatableWidget, public IServerDetailsSection
{
	GENERATED_BODY()

public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* ServerOwnerHeader;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* OwnerEmptyListContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* NonOwnerEmptyListContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonListView* ListView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* Alert;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UIconTextLoadingButton* AddFriendsButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* AddFriendsPopupContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonListView* AddFriendsListView;

private:
	// Updates the list of allows. Doesn't contact a server. This is just for updating UI.
	void RefreshAllowsList(const TArray<UAllowsDataObject*>& Allows);
	
	void LoadAllowsFromServer();
	void RefreshHeaderVisibility();

	FDelegateHandle AllowsListenerHandle;
	
	UPROPERTY()
	UServerDataObject* ServerDataObject;

	UPROPERTY()
	TArray<FString> FriendsWithPendingOrAcceptedAllow;
};
