
#include "FPSMonsterGamemode.h"

#include "Kismet/GameplayStatics.h"

void AFPSMonsterGamemode::BeginPlay()
{
	Super::BeginPlay();

	/* Set timer
	   Call after 1 sec
	   repeat forever after every 1 sec
	   */
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, 
		&AFPSMonsterGamemode::CountDownTimer, 1.0f, true, 1.0f);
}

void AFPSMonsterGamemode::RestartGameplay(bool Won)
{
	if (Won) {
		ResetLevel();
	}
	else {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, 
			&AFPSMonsterGamemode::ResetLevel, 3.0f);
	}
}

void AFPSMonsterGamemode::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Gameplay");
}

void AFPSMonsterGamemode::CountDownTimer()
{

	TimerCount--;

	if (TimerCount == 0) {
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		ResetLevel();
	}
}