// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter : ModuleRules
{
	public Shooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore",
			"HeadMountedDisplay"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });
		PublicIncludePaths.AddRange(new[]
		{
			"Shooter/Public/Player",
			"Shooter/Public/UI",
			"Shooter/Public/Weapon"
		});
	}
}