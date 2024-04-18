// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/AsyncImage.h"
#include "ServerCard.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerCard : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	// The main contents of the card. Mutually exclusive from showing the create server button
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* Card;

	// The create server button. Mututally exclusive from showing the Card contents.
	// Will be shown if we have an empty server entry (which is how we cheaply show the create server button) in
	// the private server list
	UPROPERTY(meta=(BindWidget))
	UUserWidget* CreateServerButton;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerPlayersTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerDescriptionTextBlock;	

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* JoinButton;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* FavoriteTag;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* PrivateTag;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* OwnerTag;

	UPROPERTY()
	UServerDataObject* ServerDataObject = nullptr;

	UFUNCTION(BlueprintCallable)
	void OpenServerDetails();

};
