// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Player Character - Private

#include "PlayerChararcterClass.h"
#include "Camera/CameraAnim.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerChararcterClass.h"
#include "PlayerChararcterClass.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerChararcterClass::APlayerChararcterClass()
{
	this->Tags.Add("Player");
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup the player components so that they are actually initialized and have correct values
	
	//Camera Spring Arm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraSpringArm");
	CameraSpringArm->SetupAttachment(RootComponent);
	// Set the positioning of the camera
	CameraSpringArm->TargetArmLength = 500.f;
	CameraSpringArm->TargetOffset = FVector(0.f,0.f,180.f);
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->bDoCollisionTest = false;
	
	//Camera
	TPVCamera = CreateDefaultSubobject<UCameraComponent>("TPVCamera");
	TPVCamera->SetupAttachment(CameraSpringArm);
	TPVCamera->SetRelativeRotation(FRotator(-10.f,0.f,0.f));
	
	//Cylinder
	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>("Cylinder");
	Cylinder->SetupAttachment(RootComponent);
	Cylinder->SetRelativeScale3D(FVector(0.665f, 0.665f, 1.3825f));

	//Jump Timeline
	JumpMovementTimeline = CreateDefaultSubobject<UTimelineComponent>("JumpTimeline");
	static ConstructorHelpers::FObjectFinder<UCurveFloat> ZCurve(TEXT("CurveFloat'/Game/CVObstacleCourseContent/Player/JumpCurves/JumpZCurve.JumpZCurve'"));
	this->JumpZCurve = ZCurve.Object;
	static ConstructorHelpers::FObjectFinder<UCurveFloat> XCurve(TEXT("CurveFloat'/Game/CVObstacleCourseContent/Player/JumpCurves/JumpXCurve.JumpXCurve'"));
	this->JumpXCurve = XCurve.Object;
}

// interface function override to return a pointer to this object to avoid casting
APlayerChararcterClass* APlayerChararcterClass::GetPlayerREF_Implementation()
{
	return this;
}

// Called at start
void APlayerChararcterClass::BeginPlay()
{
	Super::BeginPlay();

	Gamemode = ICVOCGameModeInterface::Execute_GetGameModeREF(UGameplayStatics::GetGameMode(this));

	if(JumpXCurve->IsValidLowLevelFast() && JumpZCurve->IsValidLowLevelFast())
	{
		onTimelineUpdate.BindUFunction(this, FName("JumpFunctionUpdate"));
		onTimelineFinished.BindUFunction(this, FName("JumpTimelineFinished"));
		JumpMovementTimeline->AddInterpFloat(JumpZCurve, onTimelineUpdate, FName("ZVal"));
		JumpMovementTimeline->SetTimelineFinishedFunc(onTimelineFinished);
		JumpMovementTimeline->SetTimelineLength(2.5);
		JumpMovementTimeline->SetLooping(false);
	}
}

void APlayerChararcterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->DeltaSeconds = DeltaTime;
	// movement on tick to properly apply delta time, so performance does not impact movement speed
	if (MoveForward)
	{
		this->GetCharacterMovement()->AddInputVector(FVector(0, MovementSpeed, 0));
	}
	if(MovingRight)
	{
		this->GetCharacterMovement()->AddInputVector(FVector(HorizontalMovementSpeed * -1, 0, 0));
	}
	if(MovingLeft)
	{
		this->GetCharacterMovement()->AddInputVector(FVector(HorizontalMovementSpeed, 0, 0));
	}
}

void APlayerChararcterClass::ToggleMoveForward()
{
	if (!MovementDisabled) {
		if (!Jumping)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "togglingforward");
			if (!MoveForward)
			{
				this->MoveForward = true;
				// Start timer looping every 0.05s to call MoveForwardLoop
				GetWorld()->GetTimerManager().SetTimer(MoveForwardTimerHandle, this, &APlayerChararcterClass::MoveForwardLoop, 0.05, true);
			}
			else
			{
				this->MoveForward = false;
				// Stop move forward timer
				GetWorld()->GetTimerManager().ClearTimer(MoveForwardTimerHandle);
			}
		}
	}
}

void APlayerChararcterClass::MoveForwardLoop()
{
	// keep looping until move forward is false
	if (!MoveForward)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveForwardTimerHandle);
	}
}

void APlayerChararcterClass::MoveLeft()
{
	if (!MovementDisabled) {
		if (!Jumping)
		{
			if (!MovingHorizontally)
			{
				this->MovingHorizontally = true;
				this->MovingLeft = true;
				// Start timer looping every 0.05s to call MoveLeftLoop
				GetWorld()->GetTimerManager().SetTimer(MoveLeftTimerHandle, this, &APlayerChararcterClass::MoveLeftLoop, 0.05f, true);
			}
		}
	}
}

void APlayerChararcterClass::MoveLeftLoop()
{
	// setup trace parameters
	FCollisionQueryParams QueryParams;
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(ObjectTypeQuery1);
	FVector TraceLoc = this->GetActorLocation() + FVector(40,0,0);
	UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, TraceLoc,TraceLoc,50,50, TraceObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);
	// is the trace hits something
	if(HitResult.bBlockingHit)
	{
		//stop moving left
		GetWorld()->GetTimerManager().ClearTimer(MoveLeftTimerHandle);
		this->MovingHorizontally = false;
		this->MovingRight = false;
	}
}

void APlayerChararcterClass::MoveRight()
{
	if (!MovementDisabled) {
		if (!Jumping)
		{
			if (!MovingHorizontally)
			{
				this->MovingHorizontally = true;
				this->MovingRight = true;
				// Start timer looping every 0.05s to call MoveRightLoop
				GetWorld()->GetTimerManager().SetTimer(MoveRightTimerHandle, this, &APlayerChararcterClass::MoveRightLoop, 0.05f, true);
			}
		}
	}
}

void APlayerChararcterClass::MoveRightLoop()
{
	// setup trace parameters
	FCollisionQueryParams QueryParams;
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(ObjectTypeQuery1);
	FVector TraceLoc = this->GetActorLocation() - FVector(40,0,0);
	UKismetSystemLibrary::CapsuleTraceSingleForObjects(this, TraceLoc,TraceLoc,50,50, TraceObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);
	// if the trace hits something
	if(HitResult.bBlockingHit)
	{
		// stop moving right
		GetWorld()->GetTimerManager().ClearTimer(MoveRightTimerHandle);
		this->MovingHorizontally = false;
		this->MovingRight = false;
	}
}

// stop horizontal movement timers
void APlayerChararcterClass::StopHorizontalMovement() {
	if (!Jumping)
	{
		this->MovingHorizontally = false;
		GetWorld()->GetTimerManager().ClearTimer(MoveRightTimerHandle);
		this->MovingRight = false;
		GetWorld()->GetTimerManager().ClearTimer(MoveLeftTimerHandle);
		this->MovingLeft = false;
	}
}

// start jump delay
void APlayerChararcterClass::JumpEvent()
{
	if (!MovementDisabled) {
		if (!JumpOnCooldown)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Jump event"));
			StopHorizontalMovement();
			this->Jumping = true;
			this->JumpAnim = true;
			this->JumpOnCooldown = true;
			this->MoveForward = false;
			// setup a delay timer
			GetWorld()->GetTimerManager().SetTimer(JumpDelayTimerHandle, this, &APlayerChararcterClass::JumpDelay, JumpDelayAmount, false);
		}
	}
}

// Start jump timeline
void APlayerChararcterClass::JumpDelay() {
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Jumping"));
	LocationBeforeJump = this->GetCapsuleComponent()->GetComponentToWorld().GetLocation();
	// play the timeline, which iterates through a curve
	JumpMovementTimeline->SetPlayRate(2.5f);
	JumpMovementTimeline->SetPlaybackPosition(0, false, false);
	JumpMovementTimeline->Play();
}

// at each tick on the curve
void APlayerChararcterClass::JumpFunctionUpdate(float Zval)
{
	float Z = LocationBeforeJump.Z + Zval;
	float X = LocationBeforeJump.Y + JumpXCurve->GetFloatValue(JumpMovementTimeline->GetPlaybackPosition());
	this->SetActorLocation(FVector(LocationBeforeJump.X,X,Z),true);
}

// at the end of the jump timeline
void APlayerChararcterClass::JumpTimelineFinished()
{
	this->JumpAnim = false;
	this->Jumping = false;
	// setup a timer for a jump cooldown
	GetWorld()->GetTimerManager().SetTimer(JumpCooldownTimerHandle, this, &APlayerChararcterClass::JumpCooldown, JumpCooldownAmount, false);
}

void APlayerChararcterClass::JumpCooldown() {
	JumpOnCooldown = false;
}

void APlayerChararcterClass::Die()
{
	if(!Respawning)
	{
		Respawning = true;
		bool LevelFailed = false;
		Lives--;

		// Reset all timelines/timers/variables
		if(MoveForward)
		{
			ToggleMoveForward();
		}
		StopHorizontalMovement();
		this->JumpAnim = false;
		this->Jumping = false;
		JumpMovementTimeline->Stop();
		JumpMovementTimeline->SetNewTime(0);
		JumpTimelineFinished();
		JumpOnCooldown = false;
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		
		if(Lives <= 0)
		{
			LevelFailed = true;
			// reset variables and teleport back to the start of the level
			FVector TPLoc = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass())->GetActorLocation();
			this->LatestCheckpoint = nullptr;
			TeleportTo(TPLoc, this->GetActorRotation());
			Lives = MaxLives;
		}
		else
		{
			if(LatestCheckpoint->IsValidLowLevelFast())
			{
				// teleport to the latest checkpoint
				FVector TPLoc = LatestCheckpoint->RespawnPoint->GetComponentToWorld().GetLocation();
				TeleportTo(TPLoc, this->GetActorRotation());
			}
			else
			{
				// teleport to the start of the level
				FVector TPLoc = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass())->GetActorLocation();
				TeleportTo(TPLoc, this->GetActorRotation());
			}
		}
		// blueprint event for spawning vfx and playing sound
		DeathEvent(LevelFailed);
		Respawning = false;
	}
}




// Called to bind functionality to input
void APlayerChararcterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerChararcterClass::JumpInputEvent);
	PlayerInputComponent->BindAction("ToggleForward", IE_Pressed, this, &APlayerChararcterClass::StartMoveForwardInputEvent);
	PlayerInputComponent->BindAction("ToggleForward", IE_Released, this, &APlayerChararcterClass::StopMoveForwardInputEvent);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerChararcterClass::LeftInputEvent);
	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerChararcterClass::HorizontalInputReleased);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerChararcterClass::RightInputEvent);
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &APlayerChararcterClass::HorizontalInputReleased);
}

void APlayerChararcterClass::RightInputEvent()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		if(!MovingHorizontally)
		{
			MoveRight();
		}
	}
}

void APlayerChararcterClass::HorizontalInputReleased()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		if(MovingHorizontally)
		{
			StopHorizontalMovement();
		}
	}
}

void APlayerChararcterClass::LeftInputEvent()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		if(!MovingHorizontally)
		{
			MoveLeft();
		}
	}
}


void APlayerChararcterClass::JumpInputEvent()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		JumpEvent();
	}
}

void APlayerChararcterClass::StartMoveForwardInputEvent()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		if(!MoveForward)
		{
			ToggleMoveForward();
		}
	}
}

void APlayerChararcterClass::StopMoveForwardInputEvent()
{
	if(Gamemode->GameInputMode == Keyboard)
	{
		if(MoveForward)
		{
			ToggleMoveForward();
		}
	}
}

