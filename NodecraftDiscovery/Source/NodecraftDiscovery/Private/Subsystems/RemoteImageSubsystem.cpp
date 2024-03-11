// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/RemoteImageSubsystem.h"

#include "Subsystems/AsyncTaskFetchImage.h"
#include "ImageUtils.h"
#include "Services/GameService.h"

DEFINE_LOG_CATEGORY_STATIC(LogRemoteImageSubsystem, Log, All);

void URemoteImageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URemoteImageSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void URemoteImageSubsystem::FetchImage(FString CachedFileName, FString RemoteURL, FOnImageDownloadedDelegate& Complete)
{
	UTexture2D* LoadedTexture = LoadTextureFromDisk(CachedFileName);
	if (LoadedTexture)
	{
		Complete.Broadcast(LoadedTexture);
	}
	else
	{
		UAsyncTaskFetchImage* DownloadTask = NewObject<UAsyncTaskFetchImage>();

		DownloadTask->OnSuccess.BindWeakLambda(this, [this, Complete, CachedFileName](UTexture2D* Texture, FString URL)
		{
			Complete.Broadcast(Texture);
			SaveTextureToDisk(Texture, CachedFileName);
		});

		DownloadTask->OnFail.BindWeakLambda(this, [Complete](UTexture2D* Texture, FString URL)
		{
			Complete.Broadcast(nullptr);
		});

		DownloadTask->Start(RemoteURL);
	}
}

void URemoteImageSubsystem::SaveTextureToDisk(UTexture2D* Texture, const FString& Filename)
{
	// Assuming you have a UTexture2DDynamic* called MyDynamicTexture

	// Get the texture data
	uint8* MipData = static_cast<uint8*>(Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

	// Get the size of the texture
	int32 Width = Texture->GetSizeX();
	int32 Height = Texture->GetSizeY();

	// Create an FImage and initialize it with the TArray
	FImage Image;
	Image.Init(Width, Height, ERawImageFormat::BGRA8); // Assuming RGBA format (4 channels)

	// Copy mip[0] data to image raw data
	FMemory::Memcpy(Image.RawData.GetData(), MipData, Width * Height * 4);

	// // Unlock the texture to finalize the update
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	// Define the file path and name
	const FString FilePath = FPaths::ProjectSavedDir() + TEXT("ImageCache/") + Filename + TEXT(".png");

	// Save the image to disk
	if (FImageUtils::SaveImageByExtension(WCHAR_TO_TCHAR(*FilePath), Image))
	{
		UE_LOG(LogRemoteImageSubsystem, Verbose, TEXT("Texture saved to %s"), *FilePath);
	}
	else
	{
		UE_LOG(LogRemoteImageSubsystem, Verbose, TEXT("Failed to save texture to %s"), *FilePath);
	}
}

UTexture2D* URemoteImageSubsystem::LoadTextureFromDisk(const FString& Filename)
{
	const FString FilePath = FPaths::ProjectSavedDir() + TEXT("ImageCache/") + Filename + TEXT(".png");
	return FImageUtils::ImportFileAsTexture2D(FilePath);
}

UTexture2D* URemoteImageSubsystem::GetCachedGameBackground()
{
	return LoadTextureFromDisk(NC_CACHED_GAME_BACKGROUND_FILENAME);
}

void URemoteImageSubsystem::CacheGameBackgroundImage(bool bForce)
{
	FOnImageDownloadedDelegate Delegate;
	if (GetCachedGameBackground() == nullptr || bForce)
	{
		if (const FString URL = UGameService::Get().GetCachedGameBackgroundURL(); URL.IsEmpty() == false)
		{
			FetchImage(NC_CACHED_GAME_BACKGROUND_FILENAME, URL, Delegate);
		}
	}
}