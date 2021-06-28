// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{
    public class OpenXRHandTrackingCrossPlatform: ModuleRules
    {
        public OpenXRHandTrackingCrossPlatform(ReadOnlyTargetRules Target) 
				: base(Target)
        {
			PublicDependencyModuleNames.AddRange(
			   new string[]
			   {
					"InputDevice",
					"LiveLink",
					"LiveLinkInterface"
			   }
		   );

			var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);
            PrivateIncludePaths.AddRange(
                new string[] {
                    EngineDir + "/Source/ThirdParty/OpenXR/include"
				}
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
					"Slate",
					"HeadMountedDisplay",
					"SlateCore",
					"LiveLink",
					"LiveLinkInterface"
				}
            );
            
            if (Target.Platform == UnrealTargetPlatform.Win64 ||
               Target.Platform == UnrealTargetPlatform.Android ||
               Target.Platform == UnrealTargetPlatform.HoloLens)
            {
                PrivateDependencyModuleNames.AddRange(
                    new string[]
                    {
                        "OpenXRHMD",
                        "OpenXRInput"
                    }
                );
                PrivateDefinitions.AddRange(new string[] { "OPENXR_SUPPORTED" });
            }

            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenXR");

            if (Target.bBuildEditor == true)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }
        }
    }
}
