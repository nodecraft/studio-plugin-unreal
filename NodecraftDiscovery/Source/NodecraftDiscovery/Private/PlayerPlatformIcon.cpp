// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "PlayerPlatformIcon.h"
#include "Components/Image.h"
#include "..\Public\DeveloperSettings\NodecraftStudioIdentitySettings.h"
#include "Engine/DataTable.h"

void UPlayerPlatformIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	const UNodecraftStudioIdentitySettings* IdentitySettings = GetDefault<UNodecraftStudioIdentitySettings>();
	UTexture2D* IdentityIconTexture = *IdentitySettings->IconMapWrapper.IconMap.Find(Identity);
	IconImage->SetBrushFromTexture(IdentityIconTexture, false);
}

void UPlayerPlatformIcon::SetIdentity(EIdentityType Ident)
{
	Identity = Ident;
	const UNodecraftStudioIdentitySettings* IdentitySettings = GetDefault<UNodecraftStudioIdentitySettings>();
	UTexture2D* IdentityIconTexture = *IdentitySettings->IconMapWrapper.IconMap.Find(Identity);
	IconImage->SetBrushFromTexture(IdentityIconTexture, false);
}
