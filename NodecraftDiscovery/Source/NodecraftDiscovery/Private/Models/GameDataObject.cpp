// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/GameDataObject.h"

#include "Models/ServerRegionDataObject.h"

UGameDataObject* UGameDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UGameDataObject* GameDataObject = NewObject<UGameDataObject>();

	if (const TArray<TSharedPtr<FJsonValue>> Regions = Json->GetArrayField(TEXT("server_regions")); Regions.Num() > 0)
	{
		GameDataObject->ServerRegions.Empty();
		for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Regions)
		{
			UServerRegionDataObject* Region = UServerRegionDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
			GameDataObject->ServerRegions.Add(Region);
		}
	}

	const TArray<TSharedPtr<FJsonValue>> IdentTypesRaw = Json->GetArrayField(TEXT("ident_types"));
	GameDataObject->SupportedIdentTypes.Empty();
	for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> IdentTypeRaw : IdentTypesRaw)
	{
		const FString IdentTypeString = IdentTypeRaw.Get()->AsString();
		const EIdentityType IdentType = EIdentityTypeHelper::FromString(IdentTypeString);
		GameDataObject->SupportedIdentTypes.Add(IdentType);
	}
	
	Json->TryGetStringField(TEXT("image_background"), GameDataObject->ImageBackgroundURL);
	Json->TryGetStringField(TEXT("image_tile"), GameDataObject->GameLogoUrl);

	return GameDataObject;
}

TArray<UServerRegionDataObject*> UGameDataObject::GetServerRegions() const
{
	return ServerRegions;
}

TArray<EIdentityType> UGameDataObject::GetSupportedIdentTypes() const
{
	return SupportedIdentTypes;
}

FString UGameDataObject::GetImageBackgroundURL() const
{
	return ImageBackgroundURL;
}

FString UGameDataObject::GetGameLogoURL() const
{
	return GameLogoUrl;
}
