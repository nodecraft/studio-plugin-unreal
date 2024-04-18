// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

using UnrealBuildTool;

public class NodecraftStudioEditor : ModuleRules
{
	public NodecraftStudioEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				
				"Core",
				"EditorFramework",
				"UnrealEd",
				"DeveloperSettings",
				"UMG", 
				"CommonUI",
				// "NodecraftDiscovery",
				"Blutility",
				"SlateCore",
				"HTTP",
				"NodecraftCommon"
				// ... add other public dependencies that you statically link with here ...
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"JsonUtilities",
				"Json", 
				"NodecraftDiscovery",
				"Blutility"
				// ... add private dependencies that you statically link with here ...	
			});
		
		// if(Target.bBuildEditor)
		// {
		// 	//reference the module "MyModule"
		// 	PrivateDependencyModuleNames.Add("Blutility");
		// }
		
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