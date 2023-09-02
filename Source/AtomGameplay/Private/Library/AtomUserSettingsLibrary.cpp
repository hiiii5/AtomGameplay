// @AngelGuard


#include "Library/AtomUserSettingsLibrary.h"

#include "GameFramework/GameUserSettings.h"

EAtomUserSetting UAtomUserSettingsLibrary::StringToQualityLevel(const FString QualityLevel)
{
	// Convert the string to an enum
	if (QualityLevel == "Low")
	{
		return EAtomUserSetting::Low;
	}
	if (QualityLevel == "Medium")
	{
		return EAtomUserSetting::Medium;
	}
	if (QualityLevel == "High")
	{
		return EAtomUserSetting::High;
	}
	if (QualityLevel == "Epic")
	{
		return EAtomUserSetting::Epic;
	}
	if (QualityLevel == "Cinematic")
	{
		return EAtomUserSetting::Cinematic;
	}
	if (QualityLevel == "Custom")
	{
		return EAtomUserSetting::Custom;
	}
	return EAtomUserSetting::High;
}

EAtomUserSetting UAtomUserSettingsLibrary::IntToQualityLevel(const int32 QualityLevel)
{
	// Convert the int to an enum
	if (QualityLevel == 0)
	{
		return EAtomUserSetting::Low;
	}
	if (QualityLevel == 1)
	{
		return EAtomUserSetting::Medium;
	}
	if (QualityLevel == 2)
	{
		return EAtomUserSetting::High;
	}
	if (QualityLevel == 3)
	{
		return EAtomUserSetting::Epic;
	}
	if (QualityLevel == 4)
	{
		return EAtomUserSetting::Cinematic;
	}
	if (QualityLevel == 5)
	{
		return EAtomUserSetting::Custom;
	}
	return EAtomUserSetting::High;
}

FString UAtomUserSettingsLibrary::QualityLevelToString(const EAtomUserSetting QualityLevel)
{
	// Convert the enum to a string
	if (QualityLevel == EAtomUserSetting::Low)
	{
		return "Low";
	}
	if (QualityLevel == EAtomUserSetting::Medium)
	{
		return "Medium";
	}
	if (QualityLevel == EAtomUserSetting::High)
	{
		return "High";
	}
	if (QualityLevel == EAtomUserSetting::Epic)
	{
		return "Epic";
	}
	if (QualityLevel == EAtomUserSetting::Cinematic)
	{
		return "Cinematic";
	}
	if (QualityLevel == EAtomUserSetting::Custom)
	{
		return "Custom";
	}
	return "High";
}

int32 UAtomUserSettingsLibrary::QualityLevelToInt(const EAtomUserSetting QualityLevel)
{
	// Convert the enum to an int
	if (QualityLevel == EAtomUserSetting::Low)
	{
		return 0;
	}
	if (QualityLevel == EAtomUserSetting::Medium)
	{
		return 1;
	}
	if (QualityLevel == EAtomUserSetting::High)
	{
		return 2;
	}
	if (QualityLevel == EAtomUserSetting::Epic)
	{
		return 3;
	}
	if (QualityLevel == EAtomUserSetting::Cinematic)
	{
		return 4;
	}
	if (QualityLevel == EAtomUserSetting::Custom)
	{
		return 5;
	}
	return 2;
}

bool UAtomUserSettingsLibrary::ApplyQualitySettings(const FAtomUserSettings& Settings, const bool bCheckForCommandLineOverrides)
{
	// Get the game user settings
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogBlueprint, Warning, TEXT("UAtomUserSettingsLibrary::ApplyQualitySettings: Failed to get game user settings."));
		return false;
	}
	
	GameUserSettings->SetTextureQuality(QualityLevelToInt(Settings.TexturesQuality));
	GameUserSettings->SetShadowQuality(QualityLevelToInt(Settings.ShadowsQuality));
	GameUserSettings->SetPostProcessingQuality(QualityLevelToInt(Settings.PostProcessQuality));
	GameUserSettings->SetVisualEffectQuality(QualityLevelToInt(Settings.EffectsQuality));
	GameUserSettings->SetGlobalIlluminationQuality(QualityLevelToInt(Settings.GlobalIlluminationQuality));
	GameUserSettings->SetReflectionQuality(QualityLevelToInt(Settings.ReflectionsQuality));
	GameUserSettings->SetFoliageQuality(QualityLevelToInt(Settings.FoliageQuality));
	GameUserSettings->SetAntiAliasingQuality(QualityLevelToInt(Settings.AntiAliasingQuality));
	GameUserSettings->SetViewDistanceQuality(QualityLevelToInt(Settings.ViewDistanceQuality));
	GameUserSettings->SetShadingQuality(QualityLevelToInt(Settings.ShadingQuality));

	GameUserSettings->SetResolutionScaleNormalized(Settings.ResolutionQuality);
	GameUserSettings->SetVSyncEnabled(Settings.UseVSync == 1);
	GameUserSettings->SetFrameRateLimit(Settings.FrameRateLimit);
	GameUserSettings->SetScreenResolution(Settings.Resolution);
	GameUserSettings->SetFullscreenMode(Settings.WindowMode);

	GameUserSettings->ApplySettings(bCheckForCommandLineOverrides);
	return true;
}

bool UAtomUserSettingsLibrary::GetQualitySettings(FAtomUserSettings& Settings)
{
	const UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogBlueprint, Warning, TEXT("UAtomUserSettingsLibrary::GetQualitySettings: Failed to get game user settings."));
		return false;
	}

	Settings.TexturesQuality = IntToQualityLevel(GameUserSettings->GetTextureQuality());
	Settings.ShadowsQuality = IntToQualityLevel(GameUserSettings->GetShadowQuality());
	Settings.PostProcessQuality = IntToQualityLevel(GameUserSettings->GetPostProcessingQuality());
	Settings.EffectsQuality = IntToQualityLevel(GameUserSettings->GetVisualEffectQuality());
	Settings.GlobalIlluminationQuality = IntToQualityLevel(GameUserSettings->GetGlobalIlluminationQuality());
	Settings.ReflectionsQuality = IntToQualityLevel(GameUserSettings->GetReflectionQuality());
	Settings.FoliageQuality = IntToQualityLevel(GameUserSettings->GetFoliageQuality());
	Settings.AntiAliasingQuality = IntToQualityLevel(GameUserSettings->GetAntiAliasingQuality());
	Settings.ViewDistanceQuality = IntToQualityLevel(GameUserSettings->GetViewDistanceQuality());
	Settings.ShadingQuality = IntToQualityLevel(GameUserSettings->GetShadingQuality());

	Settings.ResolutionQuality = GameUserSettings->GetResolutionScaleNormalized();
	Settings.UseVSync = GameUserSettings->IsVSyncEnabled() ? 1 : -1;
	Settings.FrameRateLimit = GameUserSettings->GetFrameRateLimit();
	Settings.Resolution = GameUserSettings->GetScreenResolution();
	Settings.WindowMode = GameUserSettings->GetFullscreenMode();

	return true;
}

bool UAtomUserSettingsLibrary::ApplyOverallQualityLevel(const EAtomUserSetting QualityLevel, const bool bCheckForCommandLineOverrides)
{
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (!GameUserSettings)
	{
		UE_LOG(LogBlueprint, Warning, TEXT("UAtomUserSettingsLibrary::ApplyOverallQualityLevel: Failed to get game user settings."));
		return false;
	}

	const int32 QualityLevelInt = QualityLevelToInt(QualityLevel);
	GameUserSettings->SetOverallScalabilityLevel(QualityLevelInt);
	GameUserSettings->ApplySettings(bCheckForCommandLineOverrides);

	return true;
}
