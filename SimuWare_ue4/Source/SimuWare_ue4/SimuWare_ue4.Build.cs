// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimuWare_ue4 : ModuleRules
{
	public SimuWare_ue4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
