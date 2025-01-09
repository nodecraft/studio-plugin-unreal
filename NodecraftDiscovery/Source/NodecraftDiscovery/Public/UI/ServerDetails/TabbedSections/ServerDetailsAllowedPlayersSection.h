// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonActivatableWidget.h"
#include "ServerDetailsSection.h"
#include "ServerDetailsAllowedPlayersSection.generated.h"

class UAllowedPlayersListView;
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
	bool ShouldProcessBackAction() const;
	bool ProcessBackAction();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonBorder* ServerOwnerHeader;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UVerticalBox* OwnerEmptyListContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UVerticalBox* NonOwnerEmptyListContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAllowedPlayersListView* AllAllowsListView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAlertMessage* Alert;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UIconTextLoadingButton* AddFriendsButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonBorder* AddFriendsPopupContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonListView* AddFriendsListView;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle AddFriendsInputActionData;

private:
	// Updates the list of allows. Doesn't contact a server. This is just for updating UI.
	void RefreshAllowsList(const TArray<UAllowsDataObject*>& Allows);
	void RegisterAddFriendsUIAction();
	void LoadAllowsFromServer();
	void RefreshHeaderVisibility();
	void SetAddFriendsPopupVisibility(const ESlateVisibility InVisibility);

	FDelegateHandle AllowsListenerHandle;
	FUIActionBindingHandle AddFriendsUIActionHandle;
	FDelegateHandle OnAddFriendsListWidgetGeneratedHandle;
	
	UPROPERTY()
	UServerDataObject* ServerDataObject;

	UPROPERTY()
	TArray<FString> FriendsWithPendingOrAcceptedAllow;
};
