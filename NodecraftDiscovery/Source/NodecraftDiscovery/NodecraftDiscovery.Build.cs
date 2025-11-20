// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NodecraftDiscovery : ModuleRules
{
	public NodecraftDiscovery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// TODO: Find a way to make this dependent on target 
		const bool bIsSteamAuthEnabled = false;
		PublicDefinitions.Add("NC_AUTH_STEAM_ENABLED=" + (bIsSteamAuthEnabled ? "1" : "0")); 

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"UMG", 
				"CommonUI",
				"CommonInput",
				"GameplayTags",
				"InputCore",
				"Json",
				"JsonUtilities",
				"HTTP",
				"NodecraftCommon", 
				"NodecraftAuthCommon",
				"NodecraftSteamAuth"
				// ... add other public dependencies that you statically link with here ...
			}
		);

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
				Path.Combine(ModuleDirectory, "Public"),
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
