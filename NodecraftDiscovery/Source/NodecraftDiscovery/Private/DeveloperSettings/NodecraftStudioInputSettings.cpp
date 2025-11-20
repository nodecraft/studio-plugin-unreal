// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "DeveloperSettings/NodecraftStudioInputSettings.h"

FName UNodecraftStudioInputSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}

FDataTableRowHandle UNodecraftStudioInputSettings::GetDefaultLeftThumbstickMenuScrollInputActionData() const
{
	return DefaultLeftThumbstickMenuScrollInputActionData;
}

FDataTableRowHandle UNodecraftStudioInputSettings::GetDefaultRightThumbstickMenuScrollInputActionData() const
{
	return DefaultRightThumbstickMenuScrollInputActionData;
}
