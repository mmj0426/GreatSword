// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class GreatSword : ModuleRules
{
	public GreatSword(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG","NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "Private") 
		});

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory, "Public"),
			Path.Combine(ModuleDirectory, "Public/Player"),
			Path.Combine(ModuleDirectory, "Public/Boss"),
			Path.Combine(ModuleDirectory, "Public/AnimNotifyState")
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
