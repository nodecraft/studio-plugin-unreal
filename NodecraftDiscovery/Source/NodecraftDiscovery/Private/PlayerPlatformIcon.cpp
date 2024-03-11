// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPlatformIcon.h"
#include "Components/Image.h"
#include "DeveloperSettings/IdentitySettings.h"
#include "Engine/DataTable.h"

void UPlayerPlatformIcon::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	const UIdentitySettings* IdentitySettings = GetDefault<UIdentitySettings>();
	UTexture2D* IdentityIconTexture = *IdentitySettings->IconMapWrapper.IconMap.Find(Identity);
	IconImage->SetBrushFromTexture(IdentityIconTexture, false);
}

void UPlayerPlatformIcon::SetIdentity(EIdentityType Ident)
{
	Identity = Ident;
	const UIdentitySettings* IdentitySettings = GetDefault<UIdentitySettings>();
	UTexture2D* IdentityIconTexture = *IdentitySettings->IconMapWrapper.IconMap.Find(Identity);
	IconImage->SetBrushFromTexture(IdentityIconTexture, false);
}
