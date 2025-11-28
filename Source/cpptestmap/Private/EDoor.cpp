//============================================================================
//
//  Autor: BAYAR SEMIH
//
//  file: EDoor.cpp
//
//============================================================================


#include "EDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEDoor::AEDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);

	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);
	

	DoorTimeline=CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

}

// Called when the game starts or when spawned
void AEDoor::BeginPlay()
{
	Super::BeginPlay();	

	LeftStart=LeftDoor->GetRelativeLocation();
	RightStart=RightDoor->GetRelativeLocation();


	if (DoorCurve)
	{
		DoorProgress.BindUFunction(this, FName("HandleDoorProgress"));
		DoorTimeline->AddInterpFloat(DoorCurve, DoorProgress);

		
		TimelineFinished.BindUFunction(this,FName("OnTimelineFinished"));
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinished);
	}

	
}

void AEDoor::HandleDoorProgress(float value)
{
	FVector LeftTarget = LeftStart + LeftDoorOffset;
	FVector RightTarget = RightStart + RightDoorOffset;

	LeftDoor->SetRelativeLocation(FMath::Lerp(LeftStart, LeftTarget, value));
	RightDoor->SetRelativeLocation(FMath::Lerp(RightStart, RightTarget, value));
}

void AEDoor::OnTimelineFinished()
{
	
}

// Called every frame
void AEDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorTimeline)
	{
		DoorTimeline->TickComponent(DeltaTime,LEVELTICK_TimeOnly,nullptr);
	}
		
}

void AEDoor::ToggleDoor()
{
	if (!DoorTimeline || !DoorCurve) return;

	if (DoorTimeline->IsPlaying()) return;
	
	if (IsOpened)
	{
		DoorTimeline->ReverseFromEnd();
		IsOpened=false;
	}
	else
	{
		DoorTimeline->PlayFromStart();
		IsOpened=true;
	}
}
