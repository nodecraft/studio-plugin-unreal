#pragma once

#include "CoreMinimal.h"
#include "Models/ServerDataObject.h"
#include "ServerListPage.generated.h"


USTRUCT()
struct FServerListPage
{
	GENERATED_BODY()

	// Uses 1-based index
	int32 ViewPageNumber = 0;

	// Is this the last view page of the server list? i.e. is it not possible to load any more pages?
	bool bIsLastViewPage = false;

	// Used to indicate that the view should include the create server button.
	// Should only ever be set to true for the first ViewPage of the Private Servers list.
	bool bPrependCreateServerButton = false;
	
	UPROPERTY()
	TArray<UServerDataObject*> Servers;
	
	UPROPERTY()
	UServerDataObject* PreviousServerPreview = nullptr;
	
	UPROPERTY()
	UServerDataObject* NextServerPreview = nullptr;

	bool bIsLoadingNextPage = false;
	bool bIsLoadingPrevPage = false;
	bool bShouldHideList = false;
};
