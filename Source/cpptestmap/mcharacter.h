//============================================================================
//
//	Autor: BAYAR SEMIH
//
//  file: MCharacter.h
//
//============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HealthComponent.h"
#include "EDPanel.h"
#include "WeaponSystem.h"
#include "InputActionValue.h"
#include "mcharacter.generated.h"

class UHealthComponent;

UCLASS()
class CPPTESTMAP_API AMCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"));
	class  UCameraComponent* PlayerCamera;


public:
	// Sets default values for this character's properties
	AMCharacter();

private:
	float HeadBobTime = 0.0f;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CustonMesh")
	USkeletalMeshComponent* BlueprintMesh;

	// Character.h içinde, örnek:
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponSystem> WeaponClass;
	
	UPROPERTY()
	AWeaponSystem* WeaponSystem;
	
	void MoveForward(const FInputActionValue& value);
	void MoveRight(const FInputActionValue& value);
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* RealodAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* IntaractAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* MoveFAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	class UInputAction* MoveRAction;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UCameraComponent* yarrak;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float WalkSpeed=300.0f;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Movement")
	float SprintSpeed=600.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="HeadBob")
	float IdleBobSpeed=0.8f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HeadBob")
	float IdleBobAmount=0.2f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HeadBob")
	float WalkBobSpeed=2.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HeadBob")
	float WalkAmount=2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Head Bob")
	float HeadBobInterpSpeed = 10.0f;
	
	void StartSprinting();
	void StopSprinting();

	void Intaract();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();
	
	void HandleFireInput(const FInputActionValue& value);
	
	void HandleReloadInput(const FInputActionValue& value);
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	                 AActor* DamageCauser);

	void BeginPlay();

	void Tick(float DeltaTime);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	UHealthComponent* HealthComp;

	

};


