//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: EDPanel.h
//
//============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cpptestmap/Public/EDoor.h"
#include "EDPanel.generated.h"

class AEDoor;

UCLASS()
class CPPTESTMAP_API AEDPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDPanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PanelMesh;
	
	UMaterialInstanceDynamic* DynamicMat;

	UPROPERTY(EditAnywhere,Category="Panel")
	float BlinkSpeed=3.f;

	float TimeElaSpeed;


	
public:	
	// Called every frame
	UFUNCTION(BlueprintCallable,Category="Panel")
	void Interect();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Panel")
	class AEDoor* LinkedDoor;
	
};