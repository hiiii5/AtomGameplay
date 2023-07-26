// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HealthLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ATOMGAMEPLAY_API UHealthLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	static void Heal(AActor* Actor, float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	static bool HealOverTime(AActor* Actor, float HealPerTick, float Duration, float TickRate, float InitialDelay);

	UFUNCTION(BlueprintCallable, Category="Health")
	static bool DamageOverTime(AActor* Actor, float DamagePerTick, float Duration, float TickRate, float InitialDelay);
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	static bool IsAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Health")
	static float GetHealth(AActor* Actor);
};
