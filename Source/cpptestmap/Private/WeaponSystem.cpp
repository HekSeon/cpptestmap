// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HealthComponent.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"
#include "Engine/DamageEvents.h"

// Sets defau1111111111111111111111111lt values
AWeaponSystem::AWeaponSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	
	WeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMash"));
	WeaponMesh->AttachToComponent(Scene,FAttachmentTransformRules::KeepRelativeTransform);

	Muzzle=CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(WeaponMesh);

}


// Called when the game starts or when spawned
void AWeaponSystem::BeginPlay()
{
	Super::BeginPlay();

	CurrentMag=MagCapacity;
}

void AWeaponSystem::Fire()
{
	if (CurrentMag<=0||GetWorld()->TimeSeconds-LastFireTime<FireRate) return;

	CurrentMag-=1;
	LastFireTime=GetWorld()->TimeSeconds;

	PlayFireEffects();

	FHitResult Hit;
	FVector Start=Muzzle->GetComponentLocation();
	FVector End=Start+Muzzle->GetForwardVector()*5000.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit,Start,End,ECC_Visibility,Params))
	{
#if !UE_BUILD_SHIPPING
		DrawDebugLine(GetWorld(), Start, Hit.Location, FColor::Green, false, 1.0f, 0, 2.0f);
		DrawDebugPoint(GetWorld(), Hit.Location, 10.f, FColor::Red, false, 1.0f);
	 
		if (AActor* HitActor = Hit.GetActor())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
			// Hasar mekanizmasını aktif edin
		}
#endif
		
		 if (AActor* HitActor=Hit.GetActor())
		 {
		     UHealthComponent* HealthComp=Cast<UHealthComponent>(
		     	HitActor->GetComponentByClass(UHealthComponent::StaticClass())
		     );
		 	if (HealthComp&&!HealthComp->IsDead())
		 	{
		 		HealthComp->TakeDamage(Damage);
		 	}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	}
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,TEXT("Fired"));
}

void AWeaponSystem::Reload()
{
	if (CurrentMag>=MagCapacity || TotalAmmo<=0) return;
	
	int NeededMag= MagCapacity-CurrentMag;
	int ReloadAmount= FMath::Min(NeededMag,TotalAmmo);

	CurrentMag+=ReloadAmount;
	TotalAmmo-=ReloadAmount;

	PlayReloadEffects();
}
// Called every frame
void AWeaponSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

