// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerCardBase.h"

FNavigationReply UServerCardBase::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

FReply UServerCardBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	OnFocusedDelegate.ExecuteIfBound();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

