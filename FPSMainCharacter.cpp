
#include "FPSMainCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "Projectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

#include "FPSMonsterGamemode.h"

// Sets default values
AFPSMainCharacter::AFPSMainCharacter() //constructor
{
	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Character Setup

	// Create capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 95.0f); 

	// Player turn and look rates
	TurnRate = 45.0f;
	LookUpRate = 45.0f;

	/* Create camera component and attach to capsule
	   Position is relative to the capsule and uses pawn rotaion
	   where ever possible 
	 */
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FPSCamera->SetupAttachment(GetCapsuleComponent()); 
	FPSCamera->AddRelativeLocation(FVector(-39.65f, 1.75f, 64.0f)); 
	FPSCamera->bUsePawnControlRotation = true; 

	/* Create player hands that only the player can see 
	   Hands are attached to the camera
	   Does not cast shadows(dynamic or otherwise) and has
	   fixed position and rotation
	 */
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	HandsMesh->SetOnlyOwnerSee(true); 
	HandsMesh->SetupAttachment(FPSCamera); 
	HandsMesh->bCastDynamicShadow = false; 
	HandsMesh->CastShadow = false; 
	HandsMesh->AddRelativeRotation(FRotator(1.9f, -19.19f, 5.2f)); 
	HandsMesh->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));  

	// Create Gun for player that only the player can see
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true); 
	//GunMesh->SetupAttachment(HandsMesh);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	// Create Muzzle for Gun 
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh); 
	MuzzleLocation->AddRelativeLocation(FVector(0.2f, 48.4f, -10.6f)); 

	// Offset from the character location for projectiles to spawn
	GunOffSet = FVector(100.0f, 0.0f, 10.0f); 
}

// Called when the game starts or when spawned
void AFPSMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attach the gun to hands when game starts (hands not yet created)
	GunMesh->AttachToComponent(HandsMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GripPoint"));

	World = GetWorld();

	AnimInstance = HandsMesh->GetAnimInstance();
}

// Called every frame
void AFPSMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup player jumping, player firing weapon and movement
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSMainCharacter::OnFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSMainCharacter::MoveRight);

	// Setup player turn and look rate movements
	PlayerInputComponent->BindAxis("Turn", this, &AFPSMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSMainCharacter::LookAtRate);
}

void AFPSMainCharacter::OnFire()
{
	// World is important
	/* Need to have reference to the world
	   otherwise projectile will not spawn
	*/
	if (World != NULL) {
		SpawnRotation = GetControlRotation();

		//SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffSet);

		if (MuzzleLocation != nullptr) {
			SpawnLocation = MuzzleLocation->GetComponentLocation();
		}
		else {
			SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffSet);
		}

		/* Defines the paramaters for the spawning the projectile
		   If projectile can collide with a nearby mesh, spawn otherwise
		   don't spawn
		 */
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = 
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		World->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);

		if (FireSound != NULL) {
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		if (FireAnimation != NULL && AnimInstance != NULL) {
			AnimInstance->Montage_Play(FireAnimation, 1.0f);
		}
	}
}

void AFPSMainCharacter::MoveForward(float value)
{
	// add movement in forward direction
	// Note back input scale is -1.0 in project settings
	if (value != 0.0f) {
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFPSMainCharacter::MoveRight(float value)
{
	// add movement in right direction
	// Note left input scale is -1.0 in project settings
	if (value != 0.0f) {
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFPSMainCharacter::TurnAtRate(float rate)
{
	/* Get frame delta in seconds and multiply with player turn rate
	   and rate(input var)
	*/
	AddControllerYawInput(rate * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSMainCharacter::LookAtRate(float rate)
{
	/* Get frame delta in seconds and multiply with player turn rate
	   and rate(input var)
	*/
	AddControllerPitchInput(rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPSMainCharacter::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f) {
		// player dies, restart game
		// get reference to game mode
		AFPSMonsterGamemode* MyGameMode = 
			Cast<AFPSMonsterGamemode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode) {
			MyGameMode->RestartGameplay(false);
		}

		Destroy();
	}
}
