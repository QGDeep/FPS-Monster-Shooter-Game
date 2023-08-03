
#include "Enemy.h"

#include "Components/BoxComponent.h"
#include "FPSMainCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach damage box component
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	// Setup AI perception and sight
	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	/* Radius at which enemy detects and looses sight of player 
	   AI remembers player for 0.1 secs based on max age func 
	   then forgets them (detection meter timer goes away)
	   */
	SightConfig->SightRadius = 1250.0f;
	SightConfig->LoseSightRadius = 1280.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f);

	/* Setup sight as main sense over hearing */
	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 375.0f;

	DistanceSquared = BIG_NUMBER;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHit);

	BaseLocation = this->GetActorLocation();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero()) {
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

		if (BackToBaseLocation) {
			// if enemy is going back to base location (start point)
			if ((NewLocation - BaseLocation).SizeSquared2D() < DistanceSquared) {
				DistanceSquared = (NewLocation - BaseLocation).SizeSquared2D();
			}
			else {
				CurrentVelocity = FVector::ZeroVector;
				DistanceSquared = BIG_NUMBER;
				BackToBaseLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}
		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnHit(UPrimitiveComponent* Hitcomp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// cast to main player char as other actor for the enemy in this function
	AFPSMainCharacter* Char = Cast<AFPSMainCharacter>(OtherActor);

	if (Char) {
		// if cast success, deal damage to player
		Char->DealDamage(DamageValue);
	}
}

void AEnemy::OnSensed(const TArray<AActor*>& UpdatedActors)
{ 
	/* Move through updated actors array and check they detect anything (player) 
	*/
	for (int i = 0; i < UpdatedActors.Num(); i++) {
		// get info about sensed actor
		FActorPerceptionBlueprintInfo Info;

		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);


		if (Info.LastSensedStimuli[i].WasSuccessfullySensed()) {
			// if player is sensed
			// as long as we are following the player
			FVector dir = UpdatedActors[i]->GetActorLocation() - GetActorLocation();
			dir.Z = 0.0f;

			CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;

			SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());
		}
		else {
			// if player is not sensed, go back to base location
			FVector dir = BaseLocation - GetActorLocation();
			dir.Z = 0.0f;

			if (dir.SizeSquared2D() > 1.0f) {
				CurrentVelocity = dir.GetUnsafeNormal() * MovementSpeed;
				BackToBaseLocation = true;

				SetNewRotation(BaseLocation, GetActorLocation());
			}
		}
	}
}

void AEnemy::SetNewRotation(FVector TagetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TagetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;

	EnemyRotation = NewDirection.Rotation();

	SetActorRotation(EnemyRotation);
}

void AEnemy::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f) {
		Destroy();
	}
}

