// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/TabbedSections/ServerDetailsModerationHistorySection.h"

#include "CommonListView.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Services/ServersService.h"
#include "Models/ModerationLogEntryDataObject.h"

DEFINE_LOG_CATEGORY_STATIC(LogServerModerationSection, All, All);

void UServerDetailsModerationHistorySection::SetServerId(const FString& InServerId)
{
	ServerId = InServerId;
}

void UServerDetailsModerationHistorySection::SetServerData(UServerDataObject* InServerDataObject)
{
	ServerDataObject = InServerDataObject;
	if (ServerDataObject == nullptr)
	{
		LoadGuard->SetIsLoading(false);
		UE_LOG(LogServerModerationSection, Error, TEXT("UServerDetailsModerationSection::SetServerData(): ServerData is null!"));
		return;
	}
}

void UServerDetailsModerationHistorySection::LoadData()
{
	ListView->ClearListItems();
 
 	FListPublicServerModerationDelegate OnComplete;
 	OnComplete.BindWeakLambda(this, [this](TArray<UModerationLogEntryDataObject*> ModerationLogEntryDataObjects, bool bSuccess, TOptional<FText> Error)
 	{
 		// todo: use custom load guard 
 		NoModerationLogImage->SetVisibility(ModerationLogEntryDataObjects.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
      	NoModerationLogText->SetVisibility(ModerationLogEntryDataObjects.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
 		ListTitle->SetVisibility(ModerationLogEntryDataObjects.Num() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
 		ListView->SetListItems(ModerationLogEntryDataObjects);
 
 		LoadGuard->SetIsLoading(false);
 		
 		if (bSuccess == false)
 		{
 			UE_LOG(LogServerModerationSection, Warning, TEXT("Error: %s"), *Error.GetValue().ToString());
 		}
 	});
 	UServersService::Get().ListPublicServerModeration(ServerId, OnComplete);
}

void UServerDetailsModerationHistorySection::NativeOnActivated()
{
	Super::NativeOnActivated();
	LoadGuard->SetIsLoading(true);
	LoadData();
}


