// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "NodecraftRadioButton.h"
#include "Components/DynamicEntryBoxBase.h"
#include "NodecraftRadioButtonGroup.generated.h"

struct FNodecraftRadioButtonConfiguration;
class UNodecraftDynamicEntryBox;
class UNodecraftRadioButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddedChildRadioButton, UNodecraftRadioButton*, EntryWidget);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftRadioButtonGroup : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UUserWidget* AddChildRadioButton(FNodecraftRadioButtonConfiguration& RadioButtonDescriptor, bool bIsChecked = false);

	void SetSelectedIndex(int32 Index);
	void SetSelectedId(const FString& Id);

	FString GetSelectedId() const;
	UUserWidget* GetSelectedEntry() const;
	TArray<UUserWidget*> GetAllChildren() const;
	UUserWidget* GetChildAt(int32 Index);
	UUserWidget* GetChildById(const FString& Id);
	int32 GetChildrenCount() const;

	bool HasAnyChildren() const;

	virtual void SynchronizeProperties() override;

	FOnRadioButtonSelected OnRadioButtonSelected;
	FOnAddedChildRadioButton OnAddedChildRadioButton;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION()
	void OnEntryWidgetGenerated(UNodecraftRadioButton* EntryWidget);
	
	bool IsEntryClassValid(TSubclassOf<UUserWidget> EntryClass) const;

	UFUNCTION()
	void HandleRadioButtonSelected(int32 ButtonIndex);
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group", meta = (BindWidget, EntryClass="UNodecraftRadioButton"))
	UNodecraftDynamicEntryBox* Container;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group")
	TSubclassOf<UNodecraftRadioButton> RadioButtonClass;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button Group", meta=(TitleProperty="Id"))
	TArray<FNodecraftRadioButtonConfiguration> PreregisteredRadioButtons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group|Appearance")
	TSubclassOf<UCommonBorderStyle> ButtonBorderStyleDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group|Appearance")
	TSubclassOf<UCommonBorderStyle> ButtonBorderStyleFocused;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FMargin ButtonBorderPaddingDefault = FMargin(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FMargin ButtonBorderPaddingFocused = FMargin(0.f);

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button Group|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ButtonTriggeringInputAction;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button Group|Input", AdvancedDisplay)
	FText TriggeringActionNameOverride;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FVector2D EntrySpacing = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FRadioButtonTextStyle PrimaryTextStyle;
	
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FRadioButtonTextStyle SecondaryTextStyle;
	
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Nodecraft UI|Radio Button Group|Appearance")
	FRadioButtonTextStyle TertiaryTextStyle;

#if WITH_EDITORONLY_DATA
	
	// Used for generating a preview of a group of radio buttons in the designer
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button Group|Debug", meta = (ClampMin = 0, ClampMax = 20))
	int32 NumDesignerPreviewEntries = 3;
	
#endif

private:
	int32 CurrentSelectionIndex = INDEX_NONE;
};
