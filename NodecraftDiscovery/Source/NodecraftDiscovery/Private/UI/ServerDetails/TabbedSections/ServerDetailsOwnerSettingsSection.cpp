// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsOwnerSettingsSection.h"

#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UServerDetailsOwnerSettingsSection::SetServerData(UServerDataObject* ServerDataObject)
{
	OpenServerControlPanelButton->SetIsEnabled(true);
	OpenServerControlPanelButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
	{
		UMenuManagerSubsystem::Get().ShowServerSettingsRedirectModal(ServerDataObject->GetId());
	});
}

void UServerDetailsOwnerSettingsSection::NativeConstruct()
{
	Super::NativeConstruct();
	OpenServerControlPanelButton->SetIsEnabled(false);
}

void UServerDetailsOwnerSettingsSection::NativeDestruct()
{
	Super::NativeDestruct();
	OpenServerControlPanelButton->OnClicked().Clear();
}
