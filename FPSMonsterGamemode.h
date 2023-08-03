
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSMonsterGamemode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_MONSTERSHOOTER_API AFPSMonsterGamemode : public AGameMode
{
	GENERATED_BODY()

public:

	// check if player lost or won
	void RestartGameplay(bool Won);

private:

	void ResetLevel();

public:

	UPROPERTY(BlueprintReadOnly)
		int TimerCount = 300.0f;

private:
	FTimerHandle CountDownTimerHandle = FTimerHandle();

	void CountDownTimer();

public:
	void BeginPlay() override;
};
