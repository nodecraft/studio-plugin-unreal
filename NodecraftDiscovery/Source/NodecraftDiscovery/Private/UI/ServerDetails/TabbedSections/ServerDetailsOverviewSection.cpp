// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/TabbedSections/ServerDetailsOverviewSection.h"

void UServerDetailsOverviewSection::SetServerData(UServerDataObject* InServerDataObject)
{
	LoadGuard->SetIsLoading(false);
	if (InServerDataObject)
	{
		OverviewText->SetText(InServerDataObject->GetSummary());
	}
	else
	{
		OverviewText->SetText(FText::FromString("Error: ServerDataObject is null"));
	}
	
}

void UServerDetailsOverviewSection::SetLoading(const bool bLoading)
{
	LoadGuard->SetIsLoading(bLoading);
	OverviewText->SetText(FText::FromString("This means that loading did indeed get called"));

}

void UServerDetailsOverviewSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OverviewText->SetText(FText::FromString("This is a test"));
}
