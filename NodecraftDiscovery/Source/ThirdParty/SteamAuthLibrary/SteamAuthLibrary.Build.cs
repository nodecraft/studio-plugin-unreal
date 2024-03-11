// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SteamAuthLibrary : ModuleRules
{
	public SteamAuthLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
        Type = ModuleType.External;
        
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "sdk", "public", "steam", "lib", "win64");
            string RedistributableBinFolder = Path.Combine(ModuleDirectory, "sdk", "redistributable_bin", "win64");

            PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "sdkencryptedappticket64.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "steam_api64.lib"));

            RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "sdkencryptedappticket64.dll"), StagedFileType.UFS);
            RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "steam_api64.dll"), StagedFileType.UFS);
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "sdk", "public", "steam", "lib", "linux64");
            string RedistributableBinFolder = Path.Combine(ModuleDirectory, "sdk", "redistributable_bin", "linux64");

            PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.so"));
            PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.so"));

            RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.so"), StagedFileType.UFS);
            RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.so"), StagedFileType.UFS);
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "sdk", "public", "steam", "lib", "osx");
            string RedistributableBinFolder = Path.Combine(ModuleDirectory, "sdk", "redistributable_bin", "osx");

            PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.dylib"));
            PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.dylib"));

            RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.dylib"), StagedFileType.UFS);
            RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.dylib"), StagedFileType.UFS);
        }

        // PublicDefinitions.Add("WITH_STEAMWORKSSDK=1");
        PublicIncludePaths.Add("$(ModuleDir)/steamheaders");
    }
}
