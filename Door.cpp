
#include "Door.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSMainCharacter.h"
#include "FPSMonsterGamemode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// setup door mesh and attach box collision to door mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	RootComponent = DoorMesh;

	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Door Collision"));
	DoorCollision->SetupAttachment(DoorMesh);

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	DoorCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnHit);
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnHit(UPrimitiveComponent* Hitcomp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// cast player are other actor
	AFPSMainCharacter* Char = Cast<AFPSMainCharacter>(OtherActor);

	if (Char) {
		AFPSMonsterGamemode* MyGameMode =
			Cast<AFPSMonsterGamemode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode) {
			MyGameMode->RestartGameplay(true);
		}
	}

}

