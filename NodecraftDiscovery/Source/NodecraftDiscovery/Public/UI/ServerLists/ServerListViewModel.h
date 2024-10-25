// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaginatedServerList.h"
#include "ServerListPage.h"
#include "API/ServerListType.h"
#include "Models/ServerDataObject.h"

#include "ServerListViewModel.generated.h"

UENUM()
enum class EServerListButtonState : uint8
{
	Default,
	Disabled,
	Loading
};

UCLASS()
class NODECRAFTDISCOVERY_API UServerListViewModel : public UObject
{
	GENERATED_BODY()

public:

	EServerListButtonState GetNextButtonState();
	EServerListButtonState GetPrevButtonState();
	bool IsLoading();

	TArray<UServerDataObject*> GetServers();
	UServerDataObject* GetPreviousServerPreview();
	UServerDataObject* GetNextServerPreview();

	void GoToNextPage(UWorld* World);
	void GoToPrevPage(UWorld* World);
	void SetListType(EServerListType InListType);
	bool ShouldShowCreateServerButton() const;
	void RefreshPage(UWorld* World);
	bool ShouldHideList() const;

	FSimpleDelegate OnPageChanged;

	UFUNCTION()
	void DisplayPage(FServerListPage ServerListPage);


protected:
	bool bIsLoading = false;
	bool bShouldShowNextButton = false;
	bool bShouldShowPreviousButton = false;
	bool bShouldShowCreateServerButton = false;
	bool bShouldHideList = false;
	
	EServerListType ListType = EServerListType::Popular;

	UPROPERTY()
	TArray<UServerDataObject*> Servers;
	UPROPERTY()
	UServerDataObject* PreviousServerPreview;
	UPROPERTY()
	UServerDataObject* NextServerPreview;

	// Indexed at 1, not 0
	// View page is distinct from the actual page number returned from our API
	// The "View Page" is the page number that the user is currently looking at.
	// Actual pages fetched from the API are expected to contain more than one "view page" of data.
	int32 CurrentViewPage = 1;
	
	EServerListButtonState NextButtonState;
	EServerListButtonState PrevButtonState;
};
