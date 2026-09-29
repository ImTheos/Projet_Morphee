using UnrealBuildTool;

public class Projet_Morphee_Editor : ModuleRules
{
    public Projet_Morphee_Editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "PropertyEditor",
                "UnrealEd"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "Projet_Morphee",
                "SlateCore"
            }
        );
    }
}