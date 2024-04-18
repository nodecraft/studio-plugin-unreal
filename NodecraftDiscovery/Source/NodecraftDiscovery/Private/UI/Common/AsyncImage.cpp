// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/AsyncImage.h"

#include "DeveloperSettings/NodecraftStudioStyleSettings.h"
#include "Services/ServiceDelegates.h"
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

void UAsyncImage::LoadImageAsyncSkipCache(const FString& URI)
{
	FSimpleServiceResponseDelegate OnComplete;
	LoadImageAsyncSkipCache(URI, OnComplete);
}

void UAsyncImage::LoadImageAsyncSkipCache(const FString& URI, FSimpleServiceResponseDelegate OnComplete)
{
	SetLoading(true);
	URemoteImageSubsystem& RemoteImageSubsystem = URemoteImageSubsystem::Get();
	FOnImageDownloadedDelegate Delegate;
	Delegate.AddLambda([this, OnComplete](UTexture* Texture)
	{
		SetBrushFromTexture(Cast<UTexture2D>(Texture));
		SetLoading(false);
		const bool bSuccess = Texture != nullptr;
		const FText Error = bSuccess ? FText::FromString("Failed to load image from given url") : FText::GetEmpty();
		OnComplete.ExecuteIfBound(bSuccess, Error);
	});
	RemoteImageSubsystem.FetchImage("", URI, Delegate);
}

void UAsyncImage::LoadPlayerAvatarAsync(const UPlayerDataObject* PlayerDataObject)
{
	if (PlayerDataObject == nullptr || PlayerDataObject->GetImageAvatarUrl().IsEmpty())
	{
		if (const TSoftObjectPtr<UTexture2D> DefaultImage = UNodecraftStudioStyleSettings::Get().GetDefaultAvatarImage(); DefaultImage.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(DefaultImage.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, DefaultImage]
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
