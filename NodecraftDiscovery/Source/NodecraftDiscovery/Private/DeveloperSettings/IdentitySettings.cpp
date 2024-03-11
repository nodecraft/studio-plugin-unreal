// Fill out your copyright notice in the Description page of Project Settings.


#include "DeveloperSettings/IdentitySettings.h"

UTexture2D* UIdentitySettings::GetIconForType(EIdentityType Type) const
{
	return *IconMapWrapper.IconMap.Find(Type);
}

FName UIdentitySettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
