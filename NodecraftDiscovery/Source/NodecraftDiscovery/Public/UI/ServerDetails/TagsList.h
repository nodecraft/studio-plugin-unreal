// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WrapBox.h"
#include "UI/Common/TagButton.h"
#include "TagsList.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UTagsList : public UWrapBox
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Nodecraft UI")
	TSoftClassPtr<UTagButton> ServerTagButtonClass;

public:
	void CreateTags(UServerDataObject* ServerDataObject);

	FSimpleDelegate OnControllerMovedUpFromFirstRow;

};
