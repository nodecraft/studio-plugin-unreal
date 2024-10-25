// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/NodecraftDelegates.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "TagButton.generated.h"

class UCommonTextBlock;
class UServerDataObject;

UENUM()
enum class ETagType : uint8
{
	TagButton,
	TagText,
	TagExtra
};
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UTagButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

public:
	void Configure(UServerDataObject* ServerDataObject, const FString& TagText, ETagType TagType);

	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	
	FOnNavigationDelegate OnNavDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Style")
	TSoftClassPtr<UCommonButtonStyle> TextTagButtonStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Nodecraft UI|Style")
	TSoftClassPtr<UCommonButtonStyle> ExtraTagButtonStyle;
};
