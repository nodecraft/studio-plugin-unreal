// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "UObject/Object.h"
#include "Engine/Engine.h"
#include "AllowsService.generated.h"

enum class EAllowStatus : uint8;
class UAllowsDataObject;
DECLARE_DELEGATE_ThreeParams(FListPlayerAllowsResponseDelegate, TArray<UAllowsDataObject*> /*Allows*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FUpdateAllowResponseDelegate, UAllowsDataObject* /*Allows*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UAllowsService& Get() { return *GEngine->GetEngineSubsystem<UAllowsService>(); }

	bool AcceptAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete);
	bool DeclineAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete);
	bool ListPlayerAllows(FListPlayerAllowsResponseDelegate OnComplete);
	bool UpdateAllow(const FString& ServerId, const FString& AllowId, const EAllowStatus Status, FUpdateAllowResponseDelegate OnComplete);
	bool ListAllowsByServer(const FString& ServerId, FListPlayerAllowsResponseDelegate OnComplete);
	bool CreateAllow(const FString& ServerId, const FString& PlayerId, FUpdateAllowResponseDelegate OnComplete);
};
