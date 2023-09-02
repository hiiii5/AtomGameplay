// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AtomUserSettingsLibrary.generated.h"

UENUM(BlueprintType)
enum class EAtomUserSetting : uint8
{
	Low = 0 UMETA(DisplayName = "Low"),
	Medium = 1 UMETA(DisplayName = "Medium"),
	High = 2 UMETA(DisplayName = "High"),
	Epic = 3 UMETA(DisplayName = "Epic"),
	Cinematic = 4 UMETA(DisplayName = "Cinematic"),
	Custom = 5 UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FAtomUserSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting TexturesQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting ShadowsQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting PostProcessQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting EffectsQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting GlobalIlluminationQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting ReflectionsQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting FoliageQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting AntiAliasingQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting ViewDistanceQuality = EAtomUserSetting::High;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	EAtomUserSetting ShadingQuality = EAtomUserSetting::High;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings", meta = (ClampMin = "0.4", ClampMax = "1.0"))
	float ResolutionQuality = 1.0f;
	/** bools are a special case, we need to use int32 = 1 for true and -1 for false to allow Atom to save them if serialized. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	int32 UseVSync = 1;
	/** 0.0f indicates no cap. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	float FrameRateLimit = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	FIntPoint Resolution = FIntPoint(1920, 1080);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|UserSettings")
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::Windowed;
};

/**
 * 
 */
UCLASS()
class ATOMGAMEPLAY_API UAtomUserSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static EAtomUserSetting StringToQualityLevel(FString QualityLevel);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static EAtomUserSetting IntToQualityLevel(int32 QualityLevel);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static FString QualityLevelToString(EAtomUserSetting QualityLevel);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static int32 QualityLevelToInt(EAtomUserSetting QualityLevel);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static bool ApplyQualitySettings(const FAtomUserSettings& Settings, bool bCheckForCommandLineOverrides);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static bool GetQualitySettings(FAtomUserSettings& Settings);

	UFUNCTION(BlueprintCallable, Category = "Atom|UserSettings")
	static bool ApplyOverallQualityLevel(EAtomUserSetting QualityLevel, bool bCheckForCommandLineOverrides);
};
