//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: SElevator.cpp
//
//============================================================================


#include "SElevator.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASElevator::ASElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Elevator"));
	RootComponent = ElevatorMesh;

	ElevatorTimeline=CreateDefaultSubobject<UTimelineComponent>(TEXT("ElevatorTimeline"));
}

// Called when the game starts or when spawned
void ASElevator::BeginPlay()
{
	Super::BeginPlay();

	StartElevator();

	StartLocation=GetActorLocation();

	TargetLocation = StartLocation + FVector(0, 0, 2000);

	if (ElevatorCurve)
	{
		ProgressFunction.BindUFunction(this,FName("HandleProgress"));
		ElevatorTimeline->AddInterpFloat(ElevatorCurve,ProgressFunction);
	}
}

void ASElevator::HandleProgress(float value)
{
	FVector NewLocation=FMath::Lerp(StartLocation,TargetLocation,value);
	SetActorLocation(NewLocation);
}

// Called every frame
void ASElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ElevatorTimeline)
	{
		ElevatorTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}

}

void ASElevator::StartElevator()
{
	if (ElevatorTimeline&&ElevatorCurve)
	{
		ElevatorTimeline->PlayFromStart();
	}
}

