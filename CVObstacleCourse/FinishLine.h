// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// A Finish Line object - public

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerChararcterClass.h"
#include "CVObstacleCourseGameModeBase.h"
#include <Components/BoxComponent.h>
#include "FinishLine.generated.h"

UCLASS()
class CVOBSTACLECOURSE_API AFinishLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishLine();

	UPROPERTY()
	bool crossed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACVObstacleCourseGameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* VictorySound;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Next Level")
	FName NextLevelName;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowVictoryAnim();

	FTimerHandle NextLevelDelayTimer;
	float NextLevelDelay = 5.f;
	
	void LoadNextLevel();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

};
