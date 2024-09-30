// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class R4GD : ModuleRules
{
	public R4GD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "NavigationSystem", "UMG", "Niagara", "OnlineSubsystem", "OnlineSubsystemSteam", "Slate", "SlateCore", "MediaAssets", "GeometryCollectionEngine" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //PrivateIncludePaths.Add("ThirdParty/Steamworks/Steamv157/sdk/public");
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
