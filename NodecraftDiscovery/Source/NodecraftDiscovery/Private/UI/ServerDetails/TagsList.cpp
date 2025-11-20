// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TagsList.h"

#include "Models/ServerDataObject.h"

void UTagsList::CreateTags(UServerDataObject* ServerDataObject)
{
	if (ServerTagButtonClass.IsNull() == false)
	{
		ClearChildren();
		for (FString Tag : ServerDataObject->GetTags())
		{
			UClass* ButtonClass = ServerTagButtonClass.LoadSynchronous();
			UTagButton* TagWidget = CreateWidget<UTagButton>(this, ButtonClass);
			TagWidget->Configure(ServerDataObject, Tag, ETagType::TagButton);
			AddChild(TagWidget);
		}
	}
}
