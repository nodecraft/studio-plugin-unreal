// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NodecraftDiscovery : ModuleRules
{
	public NodecraftDiscovery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// TODO: Find a way to make this dependent on target 
		const bool bIsSteamAuthEnabled = true;
		PublicDefinitions.Add("NC_AUTH_STEAM_ENABLED=" + (bIsSteamAuthEnabled ? "1" : "0")); 

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"UMG", 
				"CommonUI",
				"Json",
				"JsonUtilities",
				"HTTP",
				"NodecraftCommon",
				// ... add other public dependencies that you statically link with here ...
			}
		);

		if (bIsSteamAuthEnabled)
		{
			PublicDependencyModuleNames.Add("SteamAuth");
		}


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ImageCore",
				// ... add private dependencies that you statically link with here ...	
			});
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);

		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
