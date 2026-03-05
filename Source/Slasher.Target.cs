// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SlasherTarget : TargetRules
{
	public SlasherTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("Slasher");
		
		// Only keep this - enables trace module
		bEnableTrace = false;
		
		// Remove bForceEnableTrace (causes conflicts with binary engine)
		// Remove GlobalDefinitions (STATS, ENABLE_STATNAMEDEVENTS already enabled in Development)
		// Remove BuildEnvironment.Unique (requires source engine)
	}
}