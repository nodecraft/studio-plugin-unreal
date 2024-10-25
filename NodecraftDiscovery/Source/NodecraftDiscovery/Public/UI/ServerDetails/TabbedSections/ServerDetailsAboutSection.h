// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "ServerDetailsSection.h"
#include "Components/ScrollBox.h"
#include "UI/ServerDetails/TagsList.h"
#include "UObject/Object.h"
#include "ServerDetailsAboutSection.generated.h"

class UNodecraftButtonBase;
class UTagButton;
class UNodecraftLoadGuard;
enum class ESocialLinkType : uint8;
class UWrapBox;
class USocialLinksList;
class UCommonListView;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsAboutSection : public UCommonUserWidget, public IServerDetailsSection
{
	GENERATED_BODY()

private:
	FString ServerId;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* SummaryText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerCreatedByText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerTypeCreatedByText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* ServerCreatedByButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	USocialLinksList* SocialLinksList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerTagsTitle;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UScrollBox* ContentScrollBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UTagsList* ServerTags;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	TSoftClassPtr<UTagButton> ServerTagButtonClass;
	
public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;

	void SetServerId(const FString& InServerId);

	virtual void NativeConstruct() override;

};
