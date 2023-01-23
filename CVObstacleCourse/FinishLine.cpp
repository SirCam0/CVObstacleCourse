// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Finish Line object - private

#include "FinishLine.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"
#include "Sound/SoundBase.h"

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup a root component to attach other components too so that they can have the correct world location
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	this->RootComponent = SceneRoot;

	// setup a collision overlap box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CollisionBox->SetupAttachment(SceneRoot);
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::CollisionBoxBeginOverlap);

	// setup a mesh component that can visually represent the finish line in level
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);
}

// load the next level
void AFinishLine::LoadNextLevel()
{
	UGameplayStatics::OpenLevel(this, NextLevelName);
}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* gm = UGameplayStatics::GetGameMode(GetWorld());
	if (gm->Implements<UCVOCGameModeInterface>()) { // if the gm object implements the game mode interface
		// interface function to get a pointer of the correct class to an object without casting
		GameMode = ICVOCGameModeInterface::Execute_GetGameModeREF(gm);
	}
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// when overlapped by the player unlock the next level and then after a delay open that level
void AFinishLine::CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Player") && !crossed) {
		crossed = true;
		ShowVictoryAnim();
		if (OtherActor->Implements<UCVOCPlayerInterface>()) {
			APlayerChararcterClass* Player = ICVOCPlayerInterface::Execute_GetPlayerREF(OtherActor);
			Player->MoveForward = false;
			Player->MovementDisabled = true;
		}
		if(VictorySound->IsValidLowLevelFast())
		{
			// play the victory sound
			UGameplayStatics::PlaySound2D(this, VictorySound);
		}
		if(NextLevelName != "None")
		{
			// unlock the next level on the gamemode
			GameMode->UnlockLevel(NextLevelName);
		}
		// after a delay, load the next level
		GetWorld()->GetTimerManager().SetTimer(NextLevelDelayTimer, this, &AFinishLine::LoadNextLevel, NextLevelDelay, false);
	}
}
