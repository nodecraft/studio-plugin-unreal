// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Redirects/RedirectModal.h"

#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "DataTypes/LinkTypes.h"
#include "Services/LinksService.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
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
	QRCode->LoadImageAsync(UNodecraftUtility::GetQrCodeUrl(URL, LinkType), FilenameForCache+"qrcode");

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
