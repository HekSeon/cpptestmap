//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: HealthComponent.cpp
//
//============================================================================

#include "cpptestmap/Public/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health=MaxHealth;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bIsDead = false;
	
}

void UHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f || bIsDead) return;

	Health=FMath::Max(0.f,Health-DamageAmount);

	OnHealthChanged.Broadcast(Health);

	if (Health<=0.f&&!bIsDead)
	{
		bIsDead=true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::SetHealth(float Newhealth)
{
	if (bIsDead) return;

	Health=FMath::Clamp(Newhealth,0.f,MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (Health<=0.f&&!bIsDead)
	{
		bIsDead=true;
		OnDeath.Broadcast();
	}
}




// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

