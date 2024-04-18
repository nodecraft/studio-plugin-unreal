// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/AllowsViewModel.h"

UAllowsViewModel* UAllowsViewModel::FromDataObjects(const UAllowsDataObject* Allow, const UServerDataObject* Server)
{
	UAllowsViewModel* AllowsViewModel = NewObject<UAllowsViewModel>();
	AllowsViewModel->Allow = Allow;
	AllowsViewModel->Server = Server;
	return AllowsViewModel;
}

const UAllowsDataObject* UAllowsViewModel::GetAllow() const
{
	return Allow;
}

const UServerDataObject* UAllowsViewModel::GetServer() const
{
	return Server;
}
