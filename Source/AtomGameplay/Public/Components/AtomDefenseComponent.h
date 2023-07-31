﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Stats/AtomStats.h"
#include "AtomDefenseComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAtomDefenseComponent, Log, All);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATOMGAMEPLAY_API UAtomDefenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAtomDefenseComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defense")
	FDefenseStats DefenseStats;

	UFUNCTION(BlueprintCallable, Category = "Defense")
	FDamageStats CalculateDamages(FDamageStats DamageStats);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	float AccumulateDamages(FDamageStats DamageStats);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	bool SetDefenseStats(const FDataTableRowHandle DefenseData);
	
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void SetDamageResistance(const FString DamageType, const float Resistance);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	void RemoveDamageResistance(const FString DamageType);

	UFUNCTION(BlueprintCallable, Category = "Defense")
	void ClearDamageResistances();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
