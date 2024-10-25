// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UObject/Object.h"
#include "ServerDetailsModerationHistorySection.generated.h"

class UNodecraftLoadGuard;
class UHorizontalBox;
class UCommonListView;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsModerationHistorySection : public UCommonActivatableWidget, public IServerDetailsSection
{
	GENERATED_BODY()
public:
	void SetServerId(const FString& InServerId);
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	void LoadData();

protected:
	// Called when we navigate to Moderation tab. Start loading moderation data then.
	virtual void NativeOnActivated() override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UImage* NoModerationLogImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* NoModerationLogText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UHorizontalBox* ListTitle;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonListView* ListView;

private:
	FString ServerId;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
};
