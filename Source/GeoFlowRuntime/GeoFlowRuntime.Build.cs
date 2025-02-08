// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GeoFlowRuntime : ModuleRules
{
	public GeoFlowRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				"Runtime/GeometryFramework/Public",
				"Runtime/GeometryCore/Public"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...

			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{

				"Core",
				"GeometryFramework",
				"GeometryCore",
                "GeoFlowCore",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
								// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
				// ... add private dependencies that you statically link with here ...	
			}
			);

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
