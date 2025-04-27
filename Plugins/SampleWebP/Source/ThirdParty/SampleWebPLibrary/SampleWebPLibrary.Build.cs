// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SampleWebPLibrary : ModuleRules
{
	public SampleWebPLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicSystemIncludePaths.Add("$(ModuleDir)/Public");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "Libwebp.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "Libwebpdemux.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "Libwebpmux.lib"));
		}
	}
}
