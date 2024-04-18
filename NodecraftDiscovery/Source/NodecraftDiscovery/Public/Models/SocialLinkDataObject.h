// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SocialLinkDataObject.generated.h"

UENUM()
enum class ESocialLinkType : uint8
{
	Twitter = 0,
	YouTube = 1,
	Twitch = 2,
	Discord = 3,
	Store = 4,
	Website = 5,
	Custom = 6
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USocialLinkDataObject : public UObject
{
	GENERATED_BODY()

public:
	static USocialLinkDataObject* FromJson(const TSharedRef<FJsonObject> Json, ESocialLinkType InLinkType);
	static USocialLinkDataObject* Create(ESocialLinkType Type, FString URL, FText Label = FText::GetEmpty());
	
	ESocialLinkType GetLinkType() const;
	FString GetURL() const;
	FText GetLabel() const;
	
protected:
	ESocialLinkType LinkType;
	FString URL;
	FText Label;
};
