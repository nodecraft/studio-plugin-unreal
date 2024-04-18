// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/Common/NodecraftTabButtonBase.h"

#include "CommonLazyImage.h"
#include "UI/Common/NodecraftTabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NodecraftTabButtonBase)

class UObject;
struct FSlateBrush;

void UNodecraftTabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UNodecraftTabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UNodecraftTabButtonBase::SetTabLabelInfo_Implementation(const FNodecraftTabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}
