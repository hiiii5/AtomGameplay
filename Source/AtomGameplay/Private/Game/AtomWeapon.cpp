// @AngelGuard


#include "Game/AtomWeapon.h"


// Sets default values
AAtomWeapon::AAtomWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAtomWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAtomWeapon::OnFireTimerExpired()
{
	bIsFiring = false;
}

void AAtomWeapon::OnReloadTimerExpired()
{
	// To reload the weapon we need to find the max ammo in the clip and get the difference between it and the amount reloaded, so that we reduce the total ammo by the difference
	const int32 AmmoToReload = FMath::Min(WeaponInfo.MaxClip - WeaponInfo.CurrentClip, WeaponInfo.CurrentAmmo);
	WeaponInfo.CurrentAmmo -= AmmoToReload;
	WeaponInfo.CurrentClip += AmmoToReload;
	OnReloaded.Broadcast();
	
	bIsReloading = false;
}

// Called every frame
void AAtomWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const UAtomWeaponData* AAtomWeapon::GetWeaponData() const
{
	return WeaponInfo.WeaponData;
}

void AAtomWeapon::AddAmmo(int32 Amount)
{
	WeaponInfo.CurrentAmmo = FMath::Clamp(WeaponInfo.CurrentAmmo + Amount, 0, WeaponInfo.MaxAmmo);
}

bool AAtomWeapon::TryFire()
{
	if (bIsFiring)
	{
		return false;
	}

	if (WeaponInfo.CurrentAmmo <= 0)
	{
		return false;
	}

	if (WeaponInfo.CurrentAmmo < WeaponInfo.AmmoPerShot)
	{
		return false;
	}

	// Not firing so start the firing timer
	bIsFiring = true;
	WeaponInfo.CurrentAmmo -= WeaponInfo.AmmoPerShot;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AAtomWeapon::OnFireTimerExpired, WeaponInfo.FireRate, false);
	OnFired.Broadcast();
	return true;
}

bool AAtomWeapon::TryReload()
{
	if (bIsReloading)
	{
		return false;
	}

	if (WeaponInfo.CurrentAmmo <= 0)
	{
		return false;
	}

	// Not reloading so start the reloading timer
	bIsReloading = true;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AAtomWeapon::OnReloadTimerExpired, WeaponInfo.ReloadTime, false);
	return true;
}

void AAtomWeapon::ForceFire()
{
	// Clear the fire timer handle
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	OnFireTimerExpired();
}

void AAtomWeapon::ForceReload()
{
	// Clear the reload timer handle
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
	OnReloadTimerExpired();
}

