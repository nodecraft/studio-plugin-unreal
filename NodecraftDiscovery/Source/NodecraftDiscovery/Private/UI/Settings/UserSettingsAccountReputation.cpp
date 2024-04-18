// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettingsAccountReputation.h"

#include "CommonBorder.h"
#include "CommonListView.h"
#include "Components/VerticalBox.h"
#include "Models/AccountReputationEntryDataObject.h"
#include "Services/PlayerReputationService.h"
#include "UI/Common/NodecraftLoadGuard.h"

void UUserSettingsAccountReputation::NativeOnActivated()
{
	Super::NativeOnActivated();

	LoadGuard->SetIsLoading(true);
	ListView->ClearListItems();
	
	FGetPlayerReputationDelegate OnGetPlayerReputation;
	OnGetPlayerReputation.BindWeakLambda(this, [this](TArray<UAccountReputationEntryDataObject*> ReputationEntries, bool bSuccess, TOptional<FText> Error)
	{
		// todo: display an error message is bSuccess == false
		if (bSuccess)
		{
			ListView->SetListItems(ReputationEntries);
		}
		BansContent->SetVisibility(ReputationEntries.Num() > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		NoBansContent->SetVisibility(ReputationEntries.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		LoadGuard->SetIsLoading(false);
	});
	UPlayerReputationService::Get().GetPlayerReputation(OnGetPlayerReputation);
}		
