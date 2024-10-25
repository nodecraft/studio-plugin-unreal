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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* OverviewText;

	// void SetListeningForInput(bool bShouldListen);
	//
	// void UServerDetailsOverviewSection::UpdateBindings();

	/** The input action to listen for causing the rules to scroll down */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ScrollDownInputActionData;

	/** The input action to listen for causing the rules to scroll up */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ScrollUpInputActionData;

public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	void SetLoading(bool bLoading);

	virtual void NativeOnInitialized() override;

private:
	bool bIsListeningForInput = false;
	FUIActionBindingHandle ScrollDownActionHandle;
	FUIActionBindingHandle ScrollUpActionHandle;
};
