//============================================================================
//
//  Autor: BAYAR SEMIH
//
//  file: EDoor.h
//
//============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "EDoor.generated.h"

UCLASS()
class CPPTESTMAP_API AEDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDoor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightDoor;

	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere,Category="Door")
	UCurveFloat* DoorCurve;

	FOnTimelineFloat DoorProgress;

	FVector LeftStart;
	FVector RightStart;

	UPROPERTY(EditAnywhere,Category="Door")
	FVector LeftDoorOffset=FVector(0.f,-100.f,0.f);

	UPROPERTY(EditAnywhere,Category="Door")
	FVector RightDoorOffset=FVector(0.f,100.f,0.f);

	UFUNCTION()
	void HandleDoorProgress(float value);

	void OnTimelineFinished();

	bool IsOpened;

	FOnTimelineEvent TimelineFinished;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ToggleDoor();
};

