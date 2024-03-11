// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NodecraftAuthProvider.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnIdentTokenReceived, bool /*bSuccess*/, const FString& /*Token*/, const FString& /*Error*/)

// This class does not need to be modified.
UINTERFACE()
class UNodecraftAuthProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * Base class for all Nodecraft Auth Providers
 */
class NODECRAFTAUTHCOMMON_API INodecraftAuthProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetIdentTokenAsync(FOnIdentTokenReceived OnTokenReceived) = 0;
};
