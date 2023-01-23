// A Checkpoint class public

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class CVOBSTACLECOURSE_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* CollisonBox;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* RespawnPoint;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
