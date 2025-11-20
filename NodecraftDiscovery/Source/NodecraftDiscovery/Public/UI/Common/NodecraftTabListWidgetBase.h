// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CommonTabListWidgetBase.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Delegates/Delegate.h"
#include "HAL/Platform.h"
#include "Internationalization/Text.h"
#include "Styling/SlateBrush.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "UObject/NameTypes.h"

#include "NodecraftTabListWidgetBase.generated.h"

class UCommonButtonBase;
class UCommonUserWidget;
class UObject;
class UWidget;
struct FFrame;

USTRUCT(BlueprintType)
struct FNodecraftTabDescriptor
{
	GENERATED_BODY()

public:
	FNodecraftTabDescriptor()
	: bHidden(false)
	, CreatedTabContentWidget(nullptr)
	{ }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Tab Info")
	FName TabId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Tab Info")
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Tab Info")
	FSlateBrush IconBrush;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Tab Info")
	bool bHidden;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Tab Info")
	TSubclassOf<UCommonButtonBase> TabButtonType;

	//TODO NDarnell - This should become a TSoftClassPtr<>, the underlying common tab list needs to be able to handle lazy tab content construction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Tab Info")
	TSubclassOf<UCommonUserWidget> TabContentType;

	UPROPERTY(Transient)
	TObjectPtr<UWidget> CreatedTabContentWidget;
};

UINTERFACE(BlueprintType)
class UNodecraftTabButtonInterface : public UInterface
{
	GENERATED_BODY()
};

class INodecraftTabButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Nodecraft UI|Tab Button")
	void SetTabLabelInfo(const FNodecraftTabDescriptor& TabDescriptor);
};

UCLASS(Blueprintable, BlueprintType, Abstract, meta = (DisableNativeTick))
class NODECRAFTDISCOVERY_API UNodecraftTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Nodecraft UI|Tab List")
	bool GetPreregisteredTabInfo(const FName TabNameId, FNodecraftTabDescriptor& OutTabInfo);

	/** Helper method to get at all the preregistered tab infos */
	const TArray<FNodecraftTabDescriptor>& GetAllPreregisteredTabInfos() { return PreregisteredTabInfoArray; }

	// Toggles whether or not a specified tab is hidden, can only be called before the switcher is associated
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Tab List")
	void SetTabHiddenState(FName TabNameId, bool bHidden);

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Tab List")
	bool RegisterDynamicTab(const FNodecraftTabDescriptor& TabDescriptor);

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Tab List")
	bool IsFirstTabActive() const;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Tab List")
	bool IsLastTabActive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Nodecraft UI|Tab List")
	bool IsTabVisible(FName TabId);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Nodecraft UI|Tab List")
	int32 GetVisibleTabCount();

	/** Delegate broadcast when a new tab is created. Allows hook ups after creation. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTabContentCreated, FName, TabId, UCommonUserWidget*, TabWidget);
	DECLARE_EVENT_TwoParams(UNodecraftTabListWidgetBase, FOnTabContentCreatedNative, FName /* TabId */, UCommonUserWidget* /* TabWidget */);

	/** Broadcasts when a new tab is created. */
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft UI|Tab List")
	FOnTabContentCreated OnTabContentCreated;
	FOnTabContentCreatedNative OnTabContentCreatedNative;

protected:
	// UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget

	virtual void HandlePreLinkedSwitcherChanged() override;
	virtual void HandlePostLinkedSwitcherChanged() override;

	virtual void HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton) override;

private:
	void SetupTabs();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Tab List", meta=(TitleProperty="TabId", AllowPrivateAccess))
	TArray<FNodecraftTabDescriptor> PreregisteredTabInfoArray;
	
	/**
	 * Stores label info for tabs that have been registered at runtime but not yet created. 
	 * Elements are removed once they are created.
	 */
	UPROPERTY()
	TMap<FName, FNodecraftTabDescriptor> PendingTabLabelInfoMap;
};
