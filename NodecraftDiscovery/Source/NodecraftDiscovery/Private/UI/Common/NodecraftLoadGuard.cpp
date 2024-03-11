// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/NodecraftLoadGuard.h"

void UNodecraftLoadGuard::SetIsLoading(const bool bIsLoading)
{
	if (ContentSlot != nullptr)
	{
		ContentSlot->SetVisibility(bIsLoading ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}

	if (LoadingSlot != nullptr)
	{
		LoadingSlot->SetVisibility(bIsLoading ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UNodecraftLoadGuard::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsLoading(false);
}
