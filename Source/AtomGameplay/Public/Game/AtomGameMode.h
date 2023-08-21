// @AngelGuard

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AtomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ATOMGAMEPLAY_API AAtomGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
