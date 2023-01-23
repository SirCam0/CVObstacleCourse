// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Level Floor Object - public
// a collision volume to place under the level, when the player falls from the level they are reset

#include "LevelFloor.h"
#include "PlayerChararcterClass.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerStart.h"

// Sets default values
ALevelFloor::ALevelFloor()
{
 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// setup a collision box component
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	this->RootComponent = BoxCollision;
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALevelFloor::BoxBeginOverlap);
}

// Called when the game starts or when spawned
void ALevelFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// when overlapped by a player teleport them to the level start or their latest checkpoint and play the death vfx
void ALevelFloor::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->ActorHasTag("Player")) {
		APlayerChararcterClass* Player = ICVOCPlayerInterface::Execute_GetPlayerREF(OtherActor);
		PlayVFX(Player->GetActorLocation());
		Player->Die();
	}
}
