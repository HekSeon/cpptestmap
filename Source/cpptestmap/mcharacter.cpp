/*============================================================================

  Autor: BAYAR SEMIH

  file: MCharacter.cpp

============================================================================*/




#include "mcharacter.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputAction.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MovieSceneTracksComponentTypes.h"
#include "SNegativeActionButton.h"
#include "Components/CapsuleComponent.h"
#include "Misc/MapErrors.h"

UPROPERTY(VisibleAnywhere);
// Sets default values
AMCharacter::AMCharacter()
{
	
	PlayerCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);

	PlayerCamera->bUsePawnControlRotation=true;
	
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

}



void AMCharacter::MoveForward(const FInputActionValue& value)
{
	float AxisValue= value.Get<float>();
	if (Controller&& AxisValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
}

void AMCharacter::MoveRight(const FInputActionValue& value)
{
	float AxisValue = value.Get<float>();
	if (Controller && AxisValue !=0.0f)
	{
		const FRotator Rotation=Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction,AxisValue);
	}
}

void AMCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;

	BlueprintMesh=Cast<USkeletalMeshComponent>(GetDefaultSubobjectByName(TEXT("Cherecter")));
	if (BlueprintMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1,
			5,
			FColor::Red,
			TEXT("YARRAK1"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,
			5,
			FColor::Red,
			TEXT("bilmiim"));
	}
	
	
	if (APlayerController* PlayerController=Cast<APlayerController>(Controller))
	{
		if (IsValid(PlayerController->GetLocalPlayer()))
		{
			if (UEnhancedInputLocalPlayerSubsystem*
				Subsystem=ULocalPlayer::GetSubsystem
				<UEnhancedInputLocalPlayerSubsystem>
				(PlayerController->GetLocalPlayer()))
			{
				if (InputMapping)
				{
					Subsystem->AddMappingContext(InputMapping,0);
				}
			}
		}
		 // EquipWeapon();
	}

	

	FString DebugString = FString::Printf(TEXT("IdleBobAmount: %f"), IdleBobAmount);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, DebugString);
	FString DebugString2 = FString::Printf(TEXT("IdleBobSpeed: %f"), IdleBobSpeed);
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green, DebugString2);
}

void AMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCamera) return;

	float Speed = GetVelocity().Size2D(); // Yalnızca yatay hız önemli (Z zıplama vs. içermesin)
	bool bIsMoving = Speed > 10.f; // 1.f çok düşük; karakter neredeyse duruyor bile olsa bob olmasın

	FRotator CurrentRot = PlayerCamera->GetRelativeRotation();
	FRotator TargetRot = CurrentRot;

	if (bIsMoving)
	{
		// Yürüme head bob: Hızla orantılı, adım benzeri hareket
		HeadBobTime += DeltaTime;

		// Bob frekansı: Hız ne kadar fazlaysa, o kadar hızlı titrer
		float BobFrequency = FMath::Clamp(Speed / 300.f, 0.5f, 2.0f); // Normalize et (300 = max hız varsayımı)
		float BobAmount = FMath::Clamp(Speed / 500.f, 0.0f, WalkAmount); // Maks bob miktarı

		// Pitch bob: Sinüs dalgası → yukarı-aşağı
		float WalkBobPitch = FMath::Sin(HeadBobTime * BobFrequency * PI * 2.0f) * BobAmount;

		// Opsiyonel: Çok hafif roll (ayak basma hissi için)
		float WalkBobRoll = FMath::Cos(HeadBobTime * BobFrequency * PI * 2.0f) * BobAmount * 0.2f;

		TargetRot.Pitch += WalkBobPitch;
		TargetRot.Roll  += WalkBobRoll;
	}
	else
	{
		// Idle head bob: Çok hafif, yavaş
		HeadBobTime += DeltaTime;

		float IdlePitch = FMath::Sin(HeadBobTime * IdleBobSpeed) * IdleBobAmount;
		float IdleRoll  = FMath::Sin(HeadBobTime * IdleBobSpeed) * IdleBobAmount * 0.3f;

		TargetRot.Pitch += IdlePitch;
		TargetRot.Roll  += IdleRoll;
	}

	// Smooth interpolation
	FRotator SmoothedRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, HeadBobInterpSpeed);
	PlayerCamera->SetRelativeRotation(SmoothedRot);
}

void AMCharacter::StartSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed=SprintSpeed;
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Green,TEXT("Sprinting Started"));
}

void AMCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed=WalkSpeed;
}

void AMCharacter::Intaract()
{
	
	if (!PlayerCamera) return;
	
	FHitResult Hit;
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = Start + PlayerCamera->GetForwardVector() * 500.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AEDPanel* Panel = Cast<AEDPanel>(Hit.GetActor());
		if (Panel)
		{
			Panel->Interect();
		}
	}
}

void AMCharacter::Fire()
{
	if (IsValid(WeaponClass))
	{
		if (!WeaponSystem)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			WeaponSystem = GetWorld()->SpawnActor<AWeaponSystem>(
				WeaponClass,
				GetActorLocation(),
				GetActorRotation(),
				SpawnParams
			);

			
		}

		if (WeaponSystem)
		{
			WeaponSystem->AttachToComponent(BlueprintMesh,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				TEXT("hold")
				);
			
			
			WeaponSystem->Fire();  // Silahın kendi Fire() fonksiyonunu çağır
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,TEXT("YARRAK"));
	}
}

void AMCharacter::Reload()
{
	WeaponSystem->Reload();
}

void AMCharacter::HandleFireInput(const FInputActionValue& value)
{
	Fire();
}

void AMCharacter::HandleReloadInput(const FInputActionValue& value)
{
	Reload();
}

float AMCharacter::TakeDamage(
float DamageAmount,
struct FDamageEvent const& DamageEvent,
class AController* EventInstigator,
AActor* DamageCauser
)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComp)
	{
		HealthComp->TakeDamage(DamageAmount);
	}

	return DamageAmount;
}

void AMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AMCharacter::StartSprinting);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMCharacter::StopSprinting);
		EnhancedInput->BindAction(FireAction,ETriggerEvent::Triggered,this,&AMCharacter::HandleFireInput);
		EnhancedInput->BindAction(RealodAction,ETriggerEvent::Started,this,&AMCharacter::HandleReloadInput);
		EnhancedInput->BindAction(IntaractAction, ETriggerEvent::Triggered,this,&AMCharacter::Intaract);
		EnhancedInput->BindAction(MoveFAction,ETriggerEvent::Triggered,this,&AMCharacter::MoveForward);
		EnhancedInput->BindAction(MoveRAction,ETriggerEvent::Triggered,this,&AMCharacter::MoveRight);
		
		
		
	}
	PlayerInputComponent->BindAxis("Lookup",this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this, &APawn::AddControllerYawInput);
	
}
