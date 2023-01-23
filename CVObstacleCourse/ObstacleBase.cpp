// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// An Object Base - public
// a base object with some components that all object child classes can use if they need it

#include "ObstacleBase.h"
#include "PlayerChararcterClass.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AObstacleBase::AObstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObstacleMesh");
	ObstacleMesh->SetupAttachment(RootComponent);
	ObstacleMesh->SetCollisionProfileName("OverlapAllDynamic");
	ObstacleMesh->OnComponentBeginOverlap.AddDynamic(this, &AObstacleBase::MeshBeginOverlap);
	ObstacleMesh->OnComponentHit.AddDynamic(this, &AObstacleBase::MeshHitEvent);
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObstacleBase::MeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		KillPlayer();
	}
}

void AObstacleBase::MeshHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		KillPlayer();
	}
}

void AObstacleBase::KillPlayer()
{
	AActor* PlayerPtr = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerPtr->Implements<UCVOCPlayerInterface>()) {
		APlayerChararcterClass* Player = ICVOCPlayerInterface::Execute_GetPlayerREF(PlayerPtr);
		const FRotator Rot = Player->GetActorRotation();
		Player->Die();
	}
}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

