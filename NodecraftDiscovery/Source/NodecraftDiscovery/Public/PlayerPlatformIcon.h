// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/IdentityType.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "PlayerPlatformIcon.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerPlatformIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetIdentity(EIdentityType Ident);
	
protected:	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* IconImage;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UOverlay* Container;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	EIdentityType Identity;
};
