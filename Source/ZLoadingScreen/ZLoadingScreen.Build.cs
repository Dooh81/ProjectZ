// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class ZLoadingScreen : ModuleRules
{
    public ZLoadingScreen(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PrivatePCHHeaderFile = "Public/ZLoadingScreen.h";

        PCHUsage = PCHUsageMode.UseSharedPCHs;

        PrivateIncludePaths.Add("ZLoadingScreen");

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "MoviePlayer",
                "Slate",
                "SlateCore",
                "InputCore",
                "UMG"
            }
        );
    }
}
