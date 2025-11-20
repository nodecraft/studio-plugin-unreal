// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "ServerSessionDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerSessionDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerSessionDataObject* FromJson(const TSharedPtr<FJsonObject>& Json);

	UFUNCTION(BlueprintCallable, Category = "Nodecraft Data")
	FString GetToken() const;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft Data")
	FDateTime GetDateExpires() const;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft Data")
	FString GetPlayerAccessTokenID() const;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft Data")
	FString GetServerConnection() const;

protected:
	FString Token;
	FDateTime DateExpires;
	FString ServerConnection;
	FString PlayerAccessTokenID;
};
