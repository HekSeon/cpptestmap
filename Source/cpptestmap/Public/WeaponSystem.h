// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSystem.generated.h"

UCLASS()
class CPPTESTMAP_API AWeaponSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSystem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Damage=10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float FireRate=0.02;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	int MagCapacity=30;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	TSubclassOf< UDamageType > DamageType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Muzzle;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class USceneComponent* Scene;

	

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFireEffects();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayReloadEffects();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float LastFireTime=0.f;
	int CurrentMag=30;
	int TotalAmmo=60;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

