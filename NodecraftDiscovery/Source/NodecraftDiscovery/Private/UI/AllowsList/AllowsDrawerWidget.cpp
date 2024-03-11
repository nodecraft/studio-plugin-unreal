﻿// Fill out your copyright notice in the Description page of Project Settings.


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
			ListView->SetListItems(Allows);
		}
		LoadGuard->SetIsLoading(false);
	});
	UAllowsService::Get().ListPlayerAllows(OnListPlayerAllows);
}
