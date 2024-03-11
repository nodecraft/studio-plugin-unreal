// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamAuthDeveloperSettings.h"

FString USteamAuthDeveloperSettings::GetAppId() const
{
	return AppId;
}

FName USteamAuthDeveloperSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}
