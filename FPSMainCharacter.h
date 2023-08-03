
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSMainCharacter.generated.h"

UCLASS()
class FPS_MONSTERSHOOTER_API AFPSMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public: 
	// UPROPERTY variable defs
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)  // player hands mesh
		class USkeletalMeshComponent* HandsMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)  // Player gun mesh
		class USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)  // Location from where the player shoots (bullet comes out)
		class USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)  // Player camera
		class UCameraComponent* FPSCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) // Player turn rate (speed)
		float TurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) // Player lookup speed
		float LookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay) // Offset gun from player
		FVector GunOffSet;

protected: // Functions defs (basic functions)
	void OnFire(); // When player fires weapon functions

	// Movement functions
	void MoveForward(float value);
	void MoveRight(float value);

	// Player turn and look functions
	void TurnAtRate(float rate);
	void LookAtRate(float rate);

public:
	// setup projectile class and its sound
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	// animataion that will play when player shoots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	class UAnimInstance* AnimInstance;

	class UWorld* World;
	
	// Projectile rotation and location
	FRotator SpawnRotation;
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100.0f;

public:
	void DealDamage(float DamageAmount);
};
