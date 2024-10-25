// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicEntryBoxBase.h"
#include "NodecraftDynamicEntryBox.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftDynamicEntryBox : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetEntryWidgetClass() const;
	void SetEntryWidgetClass(TSubclassOf<UUserWidget> InEntryWidgetClass);

	template <typename WidgetT = UUserWidget>
	WidgetT* CreateEntry(const TSubclassOf<WidgetT>& ExplicitEntryClass = nullptr)
	{
		TSubclassOf<UUserWidget> EntryClass = ExplicitEntryClass ? ExplicitEntryClass : EntryWidgetClass;
		if (EntryClass && EntryClass->IsChildOf(WidgetT::StaticClass()) && IsEntryClassValid(EntryClass))
		{
			return Cast<WidgetT>(CreateEntryInternal(EntryClass));
		}
		return nullptr;
	}

	/** Clear out the box entries, optionally deleting the underlying Slate widgets entirely as well. */
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Dynamic Entry Box")
	void Reset(bool bDeleteWidgets = false);

	UUserWidget* CreateEntryForDesigner(TSubclassOf<UUserWidget> InEntryClass);

	UUserWidget* GetEntryAtIndex(int32 Index) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Dynamic Entry Box")
	TSubclassOf<UUserWidget> EntryWidgetClass;
};
