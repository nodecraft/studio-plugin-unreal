// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerDetailsSection.h"
#include "ServerDetailsOwnerSettingsSection.generated.h"

class UNodecraftButtonBase;

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsOwnerSettingsSection : public UCommonActivatableWidget, public IServerDetailsSection
{
	GENERATED_BODY()

public:
	virtual void SetServerData(UServerDataObject* ServerDataObject) override;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* OpenServerControlPanelButton;
};
