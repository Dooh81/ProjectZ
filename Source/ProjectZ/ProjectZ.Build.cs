// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectZ : ModuleRules
{
	public ProjectZ(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameLiftServerSDK", "UMG", "SlateCore", "HTTP", "Json", "JsonUtilities", "WebBrowserWidget", "WebSockets", "PixelStreaming"});
        PrivateDependencyModuleNames.AddRange(new string[] { "ZLoadingScreen", "UMG", "Slate", "SlateCore" }); // ��add
    }
}
