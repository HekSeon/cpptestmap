// Designed by KINEMATION, 2025. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "Curves/CurveVector.h"
#include "CurveExtractor.generated.h"

UCLASS(Blueprintable)
class PROCEDURALCONVERTER_API UCurveExtractor : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	void SaveCurve(UPackage* Package, UCurveVector* Curve, const FString& PackagePath);

	/** Bone which will be captured */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FName BoneName;

	/** Name of procedural animation. Note: _Rot and _Loc postfixes are added automatically */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Semi, Burst or Auto"))
	FName AnimName;

	/** Weapon name */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FString WeaponName;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float SampleRate = 30.f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Should the curves be mapped [-1;1]"))
	bool bNormalize = true;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Should the curves be added to the clip"))
	bool bAddToAnimation = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition="bAddToAnimation"))
	UAnimSequence* RefAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition="bAddToAnimation"))
	FName CurveName;
	
private:
	/** Return bone transform in root bone space */
	FTransform ExtractPose(UAnimSequence* Animation, const FBoneContainer& BoneContainer,
	                                FCompactPoseBoneIndex CompactPoseBoneIndex, double Time);

	float NormalizeValue(float Value, float MaxValue) const;
	
	void ExtractCurveAssets(UAnimSequence* AnimationSequence);
	void ExtractAnimCurves(UAnimSequence* AnimationSequence);

	void AddCurveKey(TArray<FRichCurveKey>& Keys, float Time, float Value);
	void AddCurve(UAnimSequence* Animation, FName Name, const TArray<FRichCurveKey>& Keys);
	void RemoveCurve(UAnimSequence* Animation, FName Name);
	FName GetCurveName(FString Type, FString Axis) const;
};