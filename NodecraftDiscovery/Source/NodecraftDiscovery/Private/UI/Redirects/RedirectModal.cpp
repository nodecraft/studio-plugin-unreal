// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Redirects/RedirectModal.h"

#include "CommonBorder.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "DataTypes/ImageBackgroundTypes.h"
#include "DataTypes/LinkTypes.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/LinksService.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"
#include "Utility/NodecraftUtility.h"

void URedirectModal::Configure(FSimpleDelegate OnClosed, FString URL, ELinkType LinkType)
{
	switch (LinkType)
	{
	case ELinkType::Internal:
		ExternalLinkHeader->SetVisibility(ESlateVisibility::Collapsed);
		InternalLinkHeader->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		break;
	case ELinkType::External:
		ExternalLinkHeader->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		InternalLinkHeader->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default: ;
	}
	
	UrlText->SetText(FText::FromString(URL));

	// Tie the cached QR code to a URL. Clean up the filename so it doesn't contain characters that are not allowed.
	FString FilenameForCache = URL.Replace(TEXT("/"), TEXT("_"));
	FilenameForCache = FilenameForCache.Replace(TEXT(":"), TEXT("-"));
	QRCode->LoadImageAsyncSkipCache(UNodecraftUtility::GetQrCodeUrl(URL, LinkType), ETransparentPixelOverrides::FillWhite);

	OpenInBrowserButton->OnClicked().Clear();
	OpenInBrowserButton->OnClicked().AddWeakLambda(this, [this, LinkType, URL]
	{
		ULinksService::Get().OpenURLInBrowser(LinkType, URL);
	});

	CloseButtonTop->OnClicked().Clear();
	CloseButtonTop->OnClicked().AddWeakLambda(this, [OnClosed]
	{
		OnClosed.ExecuteIfBound();
	});

	CloseButtonBottom->OnClicked().Clear();
	CloseButtonBottom->OnClicked().AddWeakLambda(this, [OnClosed]
	{
		OnClosed.ExecuteIfBound();
	});
}

void URedirectModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

bool URedirectModal::NativeOnHandleBackAction()
{
	CloseButtonBottom->OnClicked().Broadcast();
	return Super::NativeOnHandleBackAction();
}

void URedirectModal::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	if (CurrentInputType == ECommonInputType::Gamepad)
	{
		if (OpenInBrowserActionData.IsNull() == false)
		{
			FBindUIActionArgs Args = FBindUIActionArgs(OpenInBrowserActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
				{
					OpenInBrowserButton->OnClicked().Broadcast();
				}));
			if (OpenInBrowserButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				Args.OverrideDisplayName = OpenInBrowserButton->GetInputActionNameOverride();
			}
			OpenInBrowserActionHandle = RegisterUIActionBinding(Args);
		}
	}
	else
	{
		if (OpenInBrowserActionHandle.IsValid())
		{
			OpenInBrowserActionHandle.Unregister();
		}
	}
}
