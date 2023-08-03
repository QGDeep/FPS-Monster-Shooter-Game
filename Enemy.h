
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class FPS_MONSTERSHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Calc damage dealt to the player
	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	// function to detect hits
	UFUNCTION()
		void OnHit(UPrimitiveComponent* Hitcomp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	// allows the enemy to see and hear the player
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAIPerceptionComponent* AIPerComp;

	// used to configure sight compnent for AI perception system
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
		class UAISenseConfig_Sight* SightConfig;

	// function that tells us when AI perception detects player
	UFUNCTION()
		void OnSensed(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FRotator EnemyRotation;

	// location the enemy returns to when they loose sight of player
	UPROPERTY(VisibleAnywhere, Category = Movement)
		FVector BaseLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		FVector CurrentVelocity;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		float MovementSpeed;

	void SetNewRotation(FVector TagetPosition, FVector CurrentPosition);

	bool BackToBaseLocation;
	FVector NewLocation;
	float DistanceSquared;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100.0f;

	UPROPERTY(EditAnywhere)
		float DamageValue = 5.0f;

public:
	void DealDamage(float DamageAmount);

};
