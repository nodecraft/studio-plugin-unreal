// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Subsystems/AsyncTaskFetchImage.h"

#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Rendering/Texture2DResource.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncTaskFetchImage)


//----------------------------------------------------------------------//
// UAsyncTaskFetchImage
//----------------------------------------------------------------------//

UAsyncTaskFetchImage::UAsyncTaskFetchImage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if ( HasAnyFlags(RF_ClassDefaultObject) == false )
	{
		AddToRoot();
	}
}

UAsyncTaskFetchImage* UAsyncTaskFetchImage::DownloadImage(FString URL)
{
	UAsyncTaskFetchImage* DownloadTask = NewObject<UAsyncTaskFetchImage>();
	DownloadTask->Start(URL);

	return DownloadTask;
}

void UAsyncTaskFetchImage::Start(FString URL)
{
#if !UE_SERVER
	// Create the Http request and add to pending request list
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UAsyncTaskFetchImage::HandleImageRequest);
	HttpRequest->SetURL(URL);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->ProcessRequest();
#else
	// On the server we don't execute fail or success we just don't fire the request.
	RemoveFromRoot();
#endif
}

void UAsyncTaskFetchImage::HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
#if !UE_SERVER

	RemoveFromRoot();

	if ( bSucceeded && HttpResponse.IsValid() && HttpResponse->GetContentLength() > 0 )
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrappers[3] =
		{
			ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG),
			ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP),
		};

		for ( auto ImageWrapper : ImageWrappers )
		{
			if ( ImageWrapper.IsValid() && ImageWrapper->SetCompressed(HttpResponse->GetContent().GetData(), HttpResponse->GetContentLength()) )
			{
				TArray64<uint8> RawData;
				const ERGBFormat InFormat = ERGBFormat::BGRA;
				if ( ImageWrapper->GetRaw(InFormat, 8, RawData) )
				{
					if (UTexture2D* Tex = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight()))
					{
						Tex->SRGB = true;
						Tex->CreateResource();
						// Tex->UpdateResource();

						// Lock the texture to update its data
						uint8* MipData = static_cast<uint8*>(Tex->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
						
						// // Copy the raw data to the texture
						FMemory::Memcpy(MipData, RawData.GetData(), RawData.Num());

						// Unlock the texture to finalize the update
						Tex->GetPlatformData()->Mips[0].BulkData.Unlock();

						OnSuccess.ExecuteIfBound(Tex, HttpRequest.Get()->GetURL());
						return;
					}
				}
			}
		}
	}

	OnFail.ExecuteIfBound(nullptr, HttpRequest.Get()->GetURL());

#endif
}

