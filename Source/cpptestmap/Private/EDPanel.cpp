//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: EDPanel.cpp
//
//============================================================================

#include "EDPanel.h"

#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AEDPanel::AEDPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PanelMesh"));
	RootComponent = PanelMesh;

}

// Called when the game starts or when spawned
void AEDPanel::BeginPlay()
{
	Super::BeginPlay();

	if (!LinkedDoor)
	{
		for (TActorIterator<AEDoor> It(GetWorld()); It; ++It)
		{
			LinkedDoor = *It;
			break;
		}
	}
	
	if (PanelMesh->GetMaterial(0))
	{
		DynamicMat=UMaterialInstanceDynamic::Create(PanelMesh->GetMaterial(0),this);
		PanelMesh->SetMaterial(0,DynamicMat);
	}
	
}

// Called every frame
void AEDPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMat)
	{
		TimeElaSpeed+=DeltaTime*BlinkSpeed;
		float EmissiveValue = (FMath::Sin(TimeElaSpeed) + 1.f) * 0.5f;
		DynamicMat->SetScalarParameterValue("EmissiveStrength",EmissiveValue * 10.f);
	}
}

void AEDPanel::Interect()
{
	if (LinkedDoor)
	{
		LinkedDoor->ToggleDoor();
	}
}

