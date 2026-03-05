using UnrealBuildTool;

public class SlasherEditor : ModuleRules
{
    public SlasherEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "AssetDefinition", "UnrealEd" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slasher", "Slate", "SlateCore" });
    }
}