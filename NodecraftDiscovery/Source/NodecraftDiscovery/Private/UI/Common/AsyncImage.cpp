// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/AsyncImage.h"

#include "DeveloperSettings/DiscoveryStyleSettings.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/RemoteImageSubsystem.h"

void UAsyncImage::LoadImageAsync(const FString& URI, const FString& FilenameForCache)
{
	SetLoading(true);
	URemoteImageSubsystem& RemoteImageSubsystem = URemoteImageSubsystem::Get();
	FOnImageDownloadedDelegate Delegate;
	Delegate.AddLambda([this](UTexture* Texture)
	{
		SetBrushFromTexture(Cast<UTexture2D>(Texture));
		SetLoading(false);
	});
	RemoteImageSubsystem.FetchImage(FilenameForCache, URI, Delegate);
}

void UAsyncImage::LoadPlayerAvatarAsync(const UPlayerDataObject* PlayerDataObject)
{
	if (PlayerDataObject == nullptr || PlayerDataObject->GetImageAvatarUrl().IsEmpty())
	{
		if (const TSoftObjectPtr<UTexture2D> DefaultImage = UDiscoveryStyleSettings::Get().GetDefaultAvatarImage(); DefaultImage.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(DefaultImage.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, DefaultImage]
		{
			SetBrushFromTexture(Cast<UTexture2D>(DefaultImage.Get()));
		}));
		}
		
	}
	else
	{
		LoadImageAsync(PlayerDataObject->GetImageAvatarUrl(), PlayerDataObject->GetImageAvatarFilenameForCache());
	}
}

void UAsyncImage::SetLoading(bool bIsLoading)
{
	SetIsLoading(bIsLoading);
}
