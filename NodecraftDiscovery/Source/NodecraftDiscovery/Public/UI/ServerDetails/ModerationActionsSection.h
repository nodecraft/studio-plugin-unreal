// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ModerationActionButton.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "ViewModels/ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationActionsSection.generated.h"

DECLARE_DELEGATE_OneParam(FOnActionSelected, EModerationAction);

UCLASS()
class NODECRAFTDISCOVERY_API UModerationActionsSection : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionButton* KickButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionButton* BanButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionButton* UnbanButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionButton* PromoteToModButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionButton* DemoteModButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

public:
	void ConfigureForPlayerServerDetails(TArray<UPlayerServerDetailsDataObject*> PlayerServerDetails);
	void SetIsLoading(bool bIsLoading);

	FOnActionSelected OnActionSelected;

	virtual void NativeOnInitialized() override;
	
};
