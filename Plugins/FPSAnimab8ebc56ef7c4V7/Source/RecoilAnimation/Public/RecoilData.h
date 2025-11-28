// Designed by KINEMATION, 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "Engine/DataAsset.h"
#include "RecoilData.generated.h"

USTRUCT(BlueprintType)
struct FRecoilSway
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector2D PitchSway{FVector2D::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector2D YawSway{FVector2D::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float RollSwayMultiplier{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float Damping{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0"))
	float Acceleration{0.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults, meta = (ClampMin="0.0", ClampMax="1.0"))
	float AdsScale{1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	FVector PivotOffset{FVector::ZeroVector};
};

USTRUCT(BlueprintType)
struct FProgression
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Accel = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Damping = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
	float Amount = 0.f;
};

USTRUCT(BlueprintType)
struct FControllerRecoilData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D HorizontalRecoilStep{FVector2D::ZeroVector};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D VerticalRecoilStep{FVector2D::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults)
	FVector2D RecoilSmoothing{FVector2D::ZeroVector};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults, meta = (ClampMin="0.0"))
	float Damping{0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Defaults, meta = (ClampMin="0.0", ClampMax="1.0"))
	float Compensation{1.f};
};

UCLASS(BlueprintType)
class RECOILANIMATION_API URecoilData : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Saves recoil animation data by creating a data asset */
	void SaveData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Controller Recoil")
	FControllerRecoilData ControllerRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* SingleRot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* SingleLoc = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* AutoRot = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advanced | Curve")
	UCurveVector* AutoLoc = nullptr;
	
	/** Target values for Pitch when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D PitchAim = FVector2D::ZeroVector;

	/** Target values for Yaw when aiming
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 YawAim = FVector4(0, 0, 0, 0);

	/** Target values for Roll when aiming
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 RollAim = FVector4(0, 0, 0, 0);

	/** Target values for Pitch when not aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D PitchHip = FVector2D::ZeroVector;

	/** Target values for Yaw when not aiming
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 YawHip = FVector4(0, 0, 0, 0);

	/** Target values for Roll when not aiming
	 * X - min min
	 * Y - min max
	 * Z - max min
	 * W - max max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector4 RollHip = FVector4(0, 0, 0, 0);

	/** Target values for Kickback when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickAim = FVector2D::ZeroVector;

	/** Target values for Kickback Right when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickAimR = FVector2D::ZeroVector;

	/** Target values for Kickback Up when aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickAimUp = FVector2D::ZeroVector;

	/** Target values for Kickback when not aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickHip = FVector2D::ZeroVector;

	/** Target values for Kickback Right when not aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickHipR = FVector2D::ZeroVector;

	/** Target values for Kickback Up when not aiming
	 * X - min
	 * Y - max
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	FVector2D KickHipUp = FVector2D::ZeroVector;

	/** Smooth amount in Auto mode. 0 means no smoothing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FRotator SmoothRot = FRotator::ZeroRotator;

	/** Smooth amount in Auto mode. 0 means no smoothing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FVector SmoothLoc = FVector::ZeroVector;

	/** Multiplier for each rotation axis while in Semi/Auto state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FRotator MultiRot = FRotator(1.f, 1.f, 1.f);

	/** Multiplier for each translation axis while in Semi/Auto state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	FVector MultiLoc = FVector(1.f, 1.f, 1.f);

	/** Enables smoothing for Roll target values */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Smoothing")
	bool bSmoothRoll = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DTarget = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DAccel = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	FVector2D Noise2DZero = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise 2D Layer")
	float NoiseAimScalar = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float Pushback = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float PushbackZero = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pushback")
	float PushbackAccel = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	FProgression PitchProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	FProgression ZProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Progression")
	float ProgressAimAlpha = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil Sway")
	FRecoilSway RecoilSway;

	/** Playback position used for getting start value during Semi-Auto transition */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
	float PlaybackOffset = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline")
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pivot")
	FVector PivotOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pivot")
	FVector AdsPivotOffset = FVector::ZeroVector;
};