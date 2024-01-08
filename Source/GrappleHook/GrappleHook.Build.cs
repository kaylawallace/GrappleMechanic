// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GrappleHook : ModuleRules
{
	public GrappleHook(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
