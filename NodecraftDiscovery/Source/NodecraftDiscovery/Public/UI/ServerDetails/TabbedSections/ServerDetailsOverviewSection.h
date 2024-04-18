// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UObject/Object.h"
#include "ServerDetailsOverviewSection.generated.h"

class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsOverviewSection : public UCommonUserWidget, public IServerDetailsSection
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OverviewText;
	
public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	void SetLoading(bool bLoading);

	virtual void NativeOnInitialized() override;
};
