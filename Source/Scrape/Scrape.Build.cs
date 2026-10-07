// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Scrape : ModuleRules
{
	public Scrape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "OpenCV" });

		PublicIncludePaths.AddRange(new string[] {
			"Scrape",
			"Scrape/Variant_Horror",
			"Scrape/Variant_Horror/UI",
			"Scrape/Variant_Shooter",
			"Scrape/Variant_Shooter/AI",
			"Scrape/Variant_Shooter/UI",
			"Scrape/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
