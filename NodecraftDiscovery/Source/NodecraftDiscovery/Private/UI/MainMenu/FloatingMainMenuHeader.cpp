// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/MainMenu/FloatingMainMenuHeader.h"

#include "Services/GameService.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "UI/Common/AsyncImage.h"

void UFloatingMainMenuHeader::NativeConstruct()
{
	Super::NativeConstruct();

	GameLogoImage->LoadImageAsync(UGameService::Get().GetCachedGameLogoURL(), NC_CACHED_GAME_LOGO_FILENAME);
}
