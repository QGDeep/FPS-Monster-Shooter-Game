
#include "Projectile.h"

#include "Components/Spherecomponent.h"
#include "GameframeWork/ProjectileMovementComponent.h"

#include "Enemy.h"

//#include "Enemy.h"

// Sets default values
AProjectile::AProjectile() //constructor
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Create sphere collision and set its radius to 20 */
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	CollisionSphere->InitSphereRadius(20.0f);

	/* Sets the sphere collision as the root for components added
	   from this point. 
	*/
	RootComponent = CollisionSphere;

	/* Create projectile and set its speed */
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	/* specifies that projectile is moving the sphere(basically, 
	   who is the projectile component moving) 
	 */
	ProjectileMovement->UpdatedComponent = CollisionSphere; 
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	/* If projectile dosn't hit anything for 3 secs, automatically destroy it */
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* Hitcomp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) // replace Hit with SweepResult
{
	// cast enemy as other actor
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy) {
		// deal damage to player
		Enemy->DealDamage(DamageValue);
		// destroy projectile after dealing damage
		Destroy();
	}
}

