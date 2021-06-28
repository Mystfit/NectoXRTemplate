// Some copyright should be here...
using System.IO;
using UnrealBuildTool;

public class DisplayRefreshRateExtension : ModuleRules
{
    public DisplayRefreshRateExtension(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = @"../../Source/OpenXRExtensions/Private/OpenXRCommon.h";

        //bEnforceIWYU = true;
        var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);

        // To detect VR Preview, not built out in packaged builds
        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UnrealEd"
                }
            );
        }

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
               
            }
        );

        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);

        PrivateIncludePaths.AddRange(
            new string[] {
                "../../OpenXRExtensions/Source/OpenXRExtensions/Private",
                "../../OpenXRExtensions/Source/OpenXRExtensions/Private/external/include/openxr"  // Include the OpenXR SDK 1.0.15 headers to use newer features
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "HeadMountedDisplay",
                "UMG",
                "OpenXRHMD"
        });

        PrivateDependencyModuleNames.AddRange(
    		new string[]
    		{
    			//"CoreUObject",
    			//"Engine",
    			"Slate",
    			"SlateCore"

    			// ... add private dependencies that you statically link with here ...	
    		}
		);
    }
}
