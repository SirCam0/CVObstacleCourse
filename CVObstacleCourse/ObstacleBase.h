// NHE2422 Assignment-2, ProjectTitle: Computer Vision Obstacle Course, Author: Cameron Greenway(22/12/22)
// An Object Base - private
// a base object with some components that all object child classes can use if they need it

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ObstacleBase.generated.h"

class APlayerCharacterClass;

UCLASS()
class CVOBSTACLECOURSE_API AObstacleBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AObstacleBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ObstacleMesh;

	UFUNCTION()
	void MeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MeshHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void KillPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
