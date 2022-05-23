// TODO: License GPL 3.0?

using UnrealBuildTool;

public class ActorTickOptimizer : ModuleRules
{
	public ActorTickOptimizer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] 
			{
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] 
			{
				"ActorTickOptimizer/Private",
                "ActorTickOptimizer/Public"
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
