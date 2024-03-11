// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/ServerRegionRow.h"

#include "CommonTextBlock.h"
#include "API/DiscoverySessionManagerSubsystem.h"
#include "Components/Image.h"
#include "Models/ServerRegionDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"


void UServerRegionRow::Configure(UServerRegionDataObject* RegionDataObject, bool bIsDefaultRegion, bool bIsSelectedRegion)
{
	RegionTitle->SetText(RegionDataObject->GetTitle());
	UpdateStyles(bIsSelectedRegion);
	
	RegionID = RegionDataObject->GetID();
	
	if (FPlayerSession PlayerSession = UDiscoverySessionManager::Get().GetPlayerSession(); PlayerSession.IsValid())
	{
		CurrentRegion->SetVisibility(PlayerSession.ServerRegionId == RegionID ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	
	DefaultText->SetVisibility(bIsDefaultRegion ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	
	Button->SetIsSelected(bIsSelectedRegion);
	ButtonStateImage->SetVisibility(bIsSelectedRegion ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	EmptyCircle->SetVisibility(bIsSelectedRegion ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	Button->OnClicked().AddWeakLambda(this, [this]()
	{
		OnServerRegionButtonClicked.ExecuteIfBound(RegionID);
	});
}

void UServerRegionRow::SetIsCurrent(bool bIsCurrent, bool bShowCurrentText)
{
	if (bShowCurrentText)
	{
		CurrentRegion->SetVisibility(bIsCurrent ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	Button->SetIsSelected(bIsCurrent);
	ButtonStateImage->SetVisibility(bIsCurrent ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	EmptyCircle->SetVisibility(bIsCurrent ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	UpdateStyles(bIsCurrent);
}

void UServerRegionRow::SetIsDefault(bool bIsDefault)
{
	DefaultText->SetVisibility(bIsDefault ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UServerRegionRow::UpdateStyles(bool bIsCurrent)
{
	if (bIsCurrent)
	{
		if (SelectedTextStyle.IsNull() == false)
		{
			FStreamableDelegate OnLoaded;
			OnLoaded.BindWeakLambda(this, [this, bIsCurrent]
			{
				RegionTitle->SetStyle(SelectedTextStyle.Get());
				ButtonStateImage->SetBrushTintColor(bIsCurrent ? ButtonStateImageSelectedColor : FColor::White);
			});
			UAssetStreamerSubsystem::Get().LoadAssetAsync(SelectedTextStyle.ToSoftObjectPath(), OnLoaded);
		}
	}
	else
	{
		if (NormalTextStyle.IsNull() == false)
		{
			FStreamableDelegate OnLoaded;
			OnLoaded.BindWeakLambda(this, [this, bIsCurrent]
			{
				RegionTitle->SetStyle(NormalTextStyle.Get());
				ButtonStateImage->SetBrushTintColor(bIsCurrent ? ButtonStateImageSelectedColor : FColor::White);
			});
			UAssetStreamerSubsystem::Get().LoadAssetAsync(NormalTextStyle.ToSoftObjectPath(), OnLoaded);
		}
	}
}

FString UServerRegionRow::GetRegionID() const
{
	return RegionID;
}

