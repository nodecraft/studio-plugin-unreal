// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/Consents.h"
#include "Models/RulesDataObject.h"
#include "UObject/Object.h"
#include "ConsentsService.generated.h"


DECLARE_DELEGATE_ThreeParams(FConsentsResponseDelegate, FGameConsents /*Consents*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FSignConsentsResponseDelegate, URulesDataObject* /*Rules*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FGetPlayerConsentsResponseDelegate, FPlayerConsents /*Consents*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UConsentsService : public UEngineSubsystem
{
	GENERATED_BODY()

	//~UEngineSubsystem interface
	// virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// virtual void Deinitialize() override;
	//~End of UEngineSubsystem interface

public:
	static UConsentsService& Get() { return *GEngine->GetEngineSubsystem<UConsentsService>(); }

	
	// Returns true if started request successfully
	bool GetGameLegalConsents(const FString& IdentType, FConsentsResponseDelegate& OnComplete);
	bool SignGameRulesConsents(const FGameConsents& Consents, FSignConsentsResponseDelegate& OnComplete);

	bool GetPlayerLegalConsents(FGetPlayerConsentsResponseDelegate& OnComplete);
	bool GetPlayerRulesConsents(FGetPlayerConsentsResponseDelegate& OnComplete);
};
