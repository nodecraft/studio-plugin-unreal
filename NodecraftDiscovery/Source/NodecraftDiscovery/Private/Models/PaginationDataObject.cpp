// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/PaginationDataObject.h"

UPaginationDataObject* UPaginationDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UPaginationDataObject* PaginationDataObject = NewObject<UPaginationDataObject>();
	PaginationDataObject->Count = Json->GetNumberField(TEXT("count"));
	PaginationDataObject->Page = Json->GetNumberField(TEXT("page"));
	PaginationDataObject->PagesMax = Json->GetNumberField(TEXT("pages_max"));
	PaginationDataObject->EntriesPerPage = Json->GetNumberField(TEXT("per_page"));
	PaginationDataObject->bInitializedFromBackendData = true;
	return PaginationDataObject;
}

int32 UPaginationDataObject::GetCount() const
{
	return Count;
}

int32 UPaginationDataObject::GetPage() const
{
	return Page;
}

int32 UPaginationDataObject::GetPagesMax() const
{
	return PagesMax;
}

int32 UPaginationDataObject::GetEntriesPerPage() const
{
	return EntriesPerPage;
}

bool UPaginationDataObject::IsUninitialized() const
{
	return bInitializedFromBackendData == false;
}	
