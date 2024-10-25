// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/AllowsList/AllowsDrawerWidget.h"

#include "CommonListView.h"
#include "Models/AllowsDataObject.h"
#include "Services/AllowsService.h"
#include "UI/Common/NodecraftLoadGuard.h"

void UAllowsDrawerWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	LoadGuard->SetIsLoading(true);
	ListView->ClearListItems();

	FListPlayerAllowsResponseDelegate OnListPlayerAllows;
	OnListPlayerAllows.BindWeakLambda(this, [this](TArray<UAllowsDataObject*> Allows, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			ListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
			{
				EntryWidget.SetFocus();
				ListView->OnEntryWidgetGenerated().RemoveAll(this);
			});
			ListView->SetListItems(Allows);
		}
		LoadGuard->SetIsLoading(false);
	});
	UAllowsService::Get().ListPlayerAllows(OnListPlayerAllows);
}

UWidget* UAllowsDrawerWidget::NativeGetDesiredFocusTarget() const
{
	if (ListView->GetDisplayedEntryWidgets().IsValidIndex(0))
	{
		return ListView->GetDisplayedEntryWidgets()[0];
	}
	return Super::NativeGetDesiredFocusTarget();
}
