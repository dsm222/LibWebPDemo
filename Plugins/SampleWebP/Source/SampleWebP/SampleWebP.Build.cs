// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SampleWebP : ModuleRules
{
	public SampleWebP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"$(ModuleDir)/Public/Core",
				"$(ModuleDir)/Public/Subsystem",
				"$(ModuleDir)/Public/Type",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"SampleWebP/Public/Log"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"SampleWebPLibrary",
				"CoreUObject",
				"Projects",
				"Engine"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"RenderCore",
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
