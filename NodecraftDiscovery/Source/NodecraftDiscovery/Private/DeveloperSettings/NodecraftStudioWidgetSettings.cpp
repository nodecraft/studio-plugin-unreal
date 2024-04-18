// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include <DeveloperSettings/NodecraftStudioWidgetSettings.h>

FName UNodecraftStudioWidgetSettings::GetCategoryName() const
{
	return FName(TEXT("Plugins"));
}

TSoftClassPtr<UCommonActivatableWidget> UNodecraftStudioWidgetSettings::GetJoiningServerQueueModal() const
{
	return JoiningServerQueueModal;
}

TSoftClassPtr<UCommonActivatableWidget> UNodecraftStudioWidgetSettings::GetServerPasswordModal() const
{
	return ServerPasswordModal;
}

TSoftClassPtr<UUserWidget> UNodecraftStudioWidgetSettings::GetMainMenuWidget() const
{
	return MainMenuWidget;
}

TSoftClassPtr<UUserWidget> UNodecraftStudioWidgetSettings::GetLoginScreenWidget() const
{
	return LoginScreenWidget;
}

TSoftClassPtr<UUserWidget> UNodecraftStudioWidgetSettings::GetInternalRedirectModal() const
{
	return InternalRedirectModal;
}

TSoftClassPtr<UUserWidget> UNodecraftStudioWidgetSettings::GetExternalRedirectModal() const
{
	return ExternalRedirectModal;
}
