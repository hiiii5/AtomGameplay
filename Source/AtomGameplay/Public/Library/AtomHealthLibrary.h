#pragma once


#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AtomHealthLibrary.generated.h"


struct FDamageStats;
class UAtomBaseDamage;
struct FDefenseData;


/**
 * 
 */
UCLASS(Blueprintable)
class ATOMGAMEPLAY_API UAtomHealthLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static void TakeDamage(AActor* Actor, float Damage, FDamageStats DamageStats);
	
	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static void Heal(AActor* Actor, float Amount);

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static bool HealOverTime(AActor* Actor, float HealPerTick, float Duration, float TickRate, float InitialDelay);

	UFUNCTION(BlueprintCallable, Category="Atom|Health")
	static bool DamageOverTime(AActor* Actor, float DamagePerTick, float Duration, float TickRate, float InitialDelay);
	
	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static bool IsAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static float GetHealth(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Atom|Health")
	static UAtomBaseDamage* CreateDamageType(FDamageStats DamageStats);
};
