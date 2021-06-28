// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class NectoXRTarget : TargetRules
{
	public NectoXRTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//CppStandard = CppStandardVersion.Cpp17;

		//Use these config options in shipping to enable logs, and to enable debugger.
        if (Configuration == UnrealTargetConfiguration.Shipping)
        {
            // BuildEnvironment = TargetBuildEnvironment.Unique;
            bUseChecksInShipping = true;
            bUseLoggingInShipping = true;
        }

		//bIncludePluginsForTargetPlatforms = true;

		ExtraModuleNames.AddRange(new string[] { "NectoXR" });
	}
}
