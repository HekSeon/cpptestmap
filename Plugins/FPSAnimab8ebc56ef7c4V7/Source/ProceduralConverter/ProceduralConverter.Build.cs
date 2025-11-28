// Designed by Hitman's Store, 2023

using UnrealBuildTool;

public class ProceduralConverter : ModuleRules
{
	public ProceduralConverter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AnimationModifiers", 
				"AnimationBlueprintLibrary",
				"BlueprintGraph",
				"RecoilAnimation",
				"AnimGraphRuntime",
				"AnimGraph",
				"GraphEditor"
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}