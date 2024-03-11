// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UObject/Object.h"
#include "ServerDetailsAboutSection.generated.h"

enum class ESocialLinkType : uint8;
class UWrapBox;
class USocialLinksList;
class UCommonListView;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UCommonLoadGuard;
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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* SummaryText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerCreatedByText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerTypeCreatedByText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ServerCreatedByButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USocialLinksList* SocialLinksList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerTagsTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* ServerTags;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftClassPtr<UNodecraftButtonBase> ServerTagButtonClass;
	
public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;

	void SetServerId(const FString& InServerId);
};
