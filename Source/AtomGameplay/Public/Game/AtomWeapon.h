// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "AtomPickup.h"
#include "AtomWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloaded);

class UAtomWeaponData;

USTRUCT(BlueprintType)
struct FAtomWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	UAtomWeaponData* WeaponData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	float Range = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	float FireRate = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	float ReloadTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	int32 MaxAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	int32 AmmoPerShot = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	int32 MaxClip = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	int32 CurrentClip = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	bool bIsReloading;
};

UCLASS(Blueprintable)
class ATOMGAMEPLAY_API AAtomWeapon : public AActor, public IAtomSaveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAtomWeapon();

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Atom|Weapon")
	FAtomWeaponInfo WeaponInfo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Atom|Weapon")
	bool bIsFiring;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Atom|Weapon")
	bool bIsReloading;

	/** Use this in blueprint to spawn effects. */
	UPROPERTY(BlueprintAssignable, Category = "Atom|Weapon")
	FOnFired OnFired;

	/** Use this in blueprint to spawn effects. */
	UPROPERTY(BlueprintAssignable, Category = "Atom|Weapon")
	FOnReloaded OnReloaded;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle FireTimerHandle;
	FTimerHandle ReloadTimerHandle;

	void OnFireTimerExpired();
	void OnReloadTimerExpired();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	const UAtomWeaponData* GetWeaponData() const;

	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	void AddAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	bool TryFire();

	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	bool TryReload();

	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	void ForceFire();
	
	UFUNCTION(BlueprintCallable, Category="Atom|Weapon")
	void ForceReload();
};
