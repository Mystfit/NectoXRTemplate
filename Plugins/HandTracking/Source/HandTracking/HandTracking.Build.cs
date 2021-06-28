// Some copyright should be here...
using System.IO;
using UnrealBuildTool;

public class HandTracking : ModuleRules
{
    public HandTracking(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
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
                        "Settings"
            }
        );

        PublicIncludePaths.AddRange(
			new string[] {
				//"HandTracking/Public",
                //"HandTracking/Public/SimpleChar",
                //"HeadMountedDisplay/Public",
                //"Runtime/Engine/Private/PhysicsEngine"
				
				// ... add public include paths required here ...
			}
            );

        PrivateIncludePaths.AddRange(
            new string[] {
                //EngineDir + "Plugins\\Runtime\\OpenXRHandTracking\\Source\\OpenXRHandTracking\\Private"

				//"HandTracking/Private",
                //"HandTracking/Private/SimpleChar",
				// ... add other private include paths required here ...
			}
            ); ;

        PublicDependencyModuleNames.AddRange(
        new string[]
        {
                    "Core",
                    "CoreUObject",
                    "InputCore",
                    "Engine",
                    "HeadMountedDisplay",
                    "UMG",
                    "OpenXRHandTrackingCrossPlatform",
                    "VRExpansionPlugin"
        });


        PrivateDependencyModuleNames.AddRange(
            new string[] 
            {
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
