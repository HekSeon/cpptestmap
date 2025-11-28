//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: HealthComponent.h
//
//============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float,Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTESTMAP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(EditAnywhere,Category="Health")
	float MaxHealth=100.0f;
	
	UPROPERTY(EditAnywhere,Category="Health")
	float Health = MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	float GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintCallable)
	void SetHealth(float Newhealth);

	bool IsDead() const {return Health <= 0;}

private:
	bool bIsDead=false;
		
};


