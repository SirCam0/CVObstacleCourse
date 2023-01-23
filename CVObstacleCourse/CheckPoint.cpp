// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Checkpoint class private

#include "CheckPoint.h"
#include "PlayerChararcterClass.h"
#include "Kismet/KismetSystemLibrary.h"

// forward declaration instead of include to prevent circular dependency
class APlayerChararcterClass;

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the collision box component
	CollisonBox = CreateDefaultSubobject<UBoxComponent>("Box");
	this->RootComponent = CollisonBox;
	CollisonBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::BoxBeginOverlap);

	// set up the respawn point scene component, this will just be used as an in world vector for teleporting to
	RespawnPoint = CreateDefaultSubobject<USceneComponent>("RespawnPoint");
	RespawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called whenever the collision box begins an overlap event
void ACheckPoint::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		/**Using interface function instead of a cast, this basically treats the object as what i am telling
		it to as i already know what it is so a costly cast is not needed*/
		APlayerChararcterClass* Player =  ICVOCPlayerInterface::Execute_GetPlayerREF(OtherActor);
		Player->LatestCheckpoint = this;
	}
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

