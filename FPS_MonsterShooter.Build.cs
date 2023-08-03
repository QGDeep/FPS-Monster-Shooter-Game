

using UnrealBuildTool;

public class FPS_MonsterShooter : ModuleRules
{
	public FPS_MonsterShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// added AIModule to enable the use of AI Perception System.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", 
			"HeadMountedDisplay", "EnhancedInput", "AIModule" });
	}
}
