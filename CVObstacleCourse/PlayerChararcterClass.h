// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// Player Character - Public

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint.h"
#include "CVObstacleCourseGameModeBase.h"
#include "Camera/CameraComponent.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "StructsAndEnums/Enums/ECurrentPosition.h"
#include "ObstacleBase.h"
#include "CVOCPlayerInterface.h"
#include "PlayerChararcterClass.generated.h"


UCLASS(Blueprintable, Category = "CVGame")
class CVOBSTACLECOURSE_API APlayerChararcterClass : public ACharacter, public ICVOCPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChararcterClass();

	APlayerChararcterClass* GetPlayerREF_Implementation() override;

	ACVObstacleCourseGameModeBase* Gamemode;

	// Define all of the components to be added to the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* TPVCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Cylinder;

	// Define all Boolean variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MoveForward = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MovingHorizontally = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Jumping = false;

	// Define all Float Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 0.34f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrafeAmount = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MovingLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MovingRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrafeTarget = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrafeAmountTemp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HorizontalMovementSpeed = 0.15f;

	// Define all int Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lives = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLives = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MovementDisabled = false;

	// Define all Vector variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocationBeforeJump = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACheckPoint* LatestCheckpoint;

	// Setup the jump timeline
	UPROPERTY()
	UTimelineComponent* JumpMovementTimeline;
	UFUNCTION()
	void JumpTimelineFinished();
	void JumpCooldown();
	UFUNCTION()
	void JumpFunctionUpdate(float Zval);
	FOnTimelineFloat onTimelineUpdate{};
	FOnTimelineEventStatic onTimelineFinished{};
	// Curves Need setting in the editor unless using constructor helpers to set the path the the asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* JumpZCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* JumpXCurve;

	// Define Timers
	FTimerHandle MoveRightTimerHandle;
	FTimerHandle MoveLeftTimerHandle;
	FTimerHandle MoveForwardTimerHandle;
	FTimerHandle JumpDelayTimerHandle;
	FTimerHandle JumpCooldownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool JumpAnim = false;

	UFUNCTION(BlueprintCallable)
	void ToggleMoveForward();
	void MoveForwardLoop();
	
	UFUNCTION(BlueprintCallable)
	void MoveLeft();
	void MoveLeftLoop();

	UFUNCTION(BlueprintCallable)
	void MoveRight();
	void MoveRightLoop();

	UFUNCTION(BlueprintCallable)
	void StopHorizontalMovement();

	UFUNCTION(BlueprintCallable)
	void JumpEvent();
	UFUNCTION(BlueprintCallable)
	void JumpDelay();

	UPROPERTY()
	float JumpDelayAmount = 0.5f;
	UPROPERTY()
	float JumpCooldownAmount = 0.5f;
	UPROPERTY()
	bool JumpOnCooldown;
	
	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION()
	void RightInputEvent();
	void HorizontalInputReleased();
	UFUNCTION()
	void LeftInputEvent();
	UFUNCTION()
	void JumpInputEvent();
	UFUNCTION()
	void StartMoveForwardInputEvent();
	void StopMoveForwardInputEvent();

	bool Respawning = false;

	// An event that can be implemented in blueprint but called from c++
	UFUNCTION(BlueprintImplementableEvent)
	void DeathEvent(bool LevelFailed);

	float DeltaSeconds;

	UFUNCTION(BlueprintImplementableEvent)
	void MoveRightBP();
	UFUNCTION(BlueprintImplementableEvent)
	void MoveLeftBP();
	UFUNCTION(BlueprintImplementableEvent)
	void MoveForwardBP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
