// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/Border.h"
#include "UI/ServerLists/ServerList.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "NodecraftStudioMainMenu.generated.h"

class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftStudioMainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* PrivateServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* RecommendedServers;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* FavoriteServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* PopularServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* GameBackground;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonActivatableWidgetStack* MenuStack;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonActivatableWidgetStack* PopupMenuStack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> ServerDetailsClass;

	UPROPERTY()
	UBorder* PopupMenuStackBackground;

	UPROPERTY()
	UBorder* MenuStackBackground;

public:
	virtual void NativeConstruct() override;

	UServerList* GetFavoriteServersList() const;
};
