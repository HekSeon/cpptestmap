// Designed by KINEMATION, 2025. All rights reserved.

#include "CurveExtractor.h"

#include "AnimationBlueprintLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "RecoilAnimation/Public/PluginSettings.h"

float UCurveExtractor::NormalizeValue(float Value, float MaxValue) const
{
	if(FMath::IsNearlyZero(MaxValue))
	{
		return 0.f;
	}

	return Value / MaxValue;
}

void UCurveExtractor::AddCurveKey(TArray<FRichCurveKey>& Keys, float Time, float Value)
{
	FRichCurveKey& Key = Keys.AddDefaulted_GetRef();
	Key.Time = Time;
	Key.Value = Value;
}

void UCurveExtractor::AddCurve(UAnimSequence* Animation, FName Name, const TArray<FRichCurveKey>& Keys)
{
	IAnimationDataController& Controller = Animation->GetController();
	const FAnimationCurveIdentifier CurveId = UAnimationCurveIdentifierExtensions::GetCurveIdentifier(
		Animation->GetSkeleton(), Name, ERawCurveTrackTypes::RCT_Float);

	if(!Keys.Num())
	{
		return;
	}
	
	// Override the curve if exists
	if(!Controller.SetCurveKeys(CurveId, Keys))
	{
		Controller.AddCurve(CurveId);
		Controller.SetCurveKeys(CurveId, Keys);
	}
}

void UCurveExtractor::RemoveCurve(UAnimSequence* Animation, FName Name)
{
	IAnimationDataController& Controller = Animation->GetController();
	const FAnimationCurveIdentifier CurveId = UAnimationCurveIdentifierExtensions::GetCurveIdentifier(
		Animation->GetSkeleton(), Name, ERawCurveTrackTypes::RCT_Float);
	
	Controller.RemoveCurve(CurveId, true);
}

FName UCurveExtractor::GetCurveName(FString Type, FString Axis) const
{
	return FName(*FString::Printf(TEXT("%s_%s_%s"), *CurveName.ToString(), *Type, *Axis));
}

void UCurveExtractor::ExtractAnimCurves(UAnimSequence* AnimationSequence)
{
	if (!AnimationSequence || !RefAnimation)
	{
		UE_LOG(LogAnimation, Error, TEXT("CurveExtractor failed. Reason: Invalid Animation"));
		return;
	}

	USkeleton* Skeleton = AnimationSequence->GetSkeleton();
	if (!Skeleton)
	{
		UE_LOG(LogAnimation, Error,
			   TEXT("CurveExtractor failed. Reason: Animation with invalid Skeleton. Animation: %s"),
			   *GetNameSafe(AnimationSequence));
		return;
	}
	
	if (!RefAnimation->GetSkeleton() || RefAnimation->GetSkeleton() != Skeleton)
	{
		UE_LOG(LogAnimation, Error,
			   TEXT("CurveExtractor failed. Reason: Animation with invalid Skeleton. Animation: %s"),
			   *GetNameSafe(RefAnimation));
		return;
	}
	
	const int32 BoneIndex = Skeleton->GetReferenceSkeleton().FindBoneIndex(BoneName);
	if (BoneIndex == INDEX_NONE)
	{
		UE_LOG(LogAnimation, Error,
			   TEXT("CurveExtractor failed. Reason: Invalid Bone Index. BoneName: %s Animation: %s Skeleton: %s"),
			   *BoneName.ToString(), *GetNameSafe(AnimationSequence), *GetNameSafe(Skeleton));
		return;
	}

	if (RefAnimation->GetSkeleton()->GetReferenceSkeleton().FindBoneIndex(BoneName) == INDEX_NONE)
	{
		UE_LOG(LogAnimation, Error,
			   TEXT("CurveExtractor failed. Reason: Invalid Bone Index. BoneName: %s Animation: %s Skeleton: %s"),
			   *BoneName.ToString(), *GetNameSafe(RefAnimation), *GetNameSafe(RefAnimation->GetSkeleton()));
		return;
	}

	FMemMark Mark(FMemStack::Get());

	TArray<FBoneIndexType> RequiredBones;
	RequiredBones.Add(BoneIndex);
	Skeleton->GetReferenceSkeleton().EnsureParentsExistAndSort(RequiredBones);

	FBoneContainer BoneContainer(RequiredBones, false, *Skeleton);
	const FCompactPoseBoneIndex CompactPoseBoneIndex = BoneContainer.
		MakeCompactPoseIndex(FMeshPoseBoneIndex(BoneIndex));

	const float AnimLength = AnimationSequence->GetPlayLength();
	const float SampleInterval = 1.f / SampleRate;

	float Time = 0.f;
	int32 SampleIndex = 0;

	FTransform RefPose = ExtractPose(RefAnimation ? RefAnimation : AnimationSequence, BoneContainer,
	                                 CompactPoseBoneIndex, 0.f);
	TArray<FTransform> BonePoses;

	FVector MaxLoc = FVector::ZeroVector;
	FRotator MaxRot = FRotator::ZeroRotator;
	
	TArray<FRichCurveKey> CurveTX;
	TArray<FRichCurveKey> CurveTY;
	TArray<FRichCurveKey> CurveTZ;
	TArray<FRichCurveKey> CurveRX;
	TArray<FRichCurveKey> CurveRY;
	TArray<FRichCurveKey> CurveRZ;

	while (Time < AnimLength)
	{
		Time = FMath::Clamp(SampleIndex * SampleInterval, 0.f, AnimLength);
		SampleIndex++;

		FTransform CurrentPose = ExtractPose(AnimationSequence, BoneContainer, CompactPoseBoneIndex, Time);
		FTransform DeltaPose;

		DeltaPose.SetLocation(CurrentPose.GetLocation() - RefPose.GetLocation());
		DeltaPose.SetRotation(CurrentPose.GetRotation() * RefPose.GetRotation().Inverse());
		DeltaPose.NormalizeRotation();

		if(bNormalize)
		{
			if (FMath::Abs(DeltaPose.GetLocation().X) > FMath::Abs(MaxLoc.X))
			{
				MaxLoc.X = FMath::Abs(DeltaPose.GetLocation().X);
			}

			if (FMath::Abs(DeltaPose.GetLocation().Y) > FMath::Abs(MaxLoc.Y))
			{
				MaxLoc.Y = FMath::Abs(DeltaPose.GetLocation().Y);
			}

			if (FMath::Abs(DeltaPose.GetLocation().Z) > FMath::Abs(MaxLoc.Z))
			{
				MaxLoc.Z = FMath::Abs(DeltaPose.GetLocation().Z);
			}

			if (FMath::Abs(DeltaPose.Rotator().Pitch) > FMath::Abs(MaxRot.Pitch))
			{
				MaxRot.Pitch = FMath::Abs(DeltaPose.Rotator().Pitch);
			}

			if (FMath::Abs(DeltaPose.Rotator().Yaw) > FMath::Abs(MaxRot.Yaw))
			{
				MaxRot.Yaw = FMath::Abs(DeltaPose.Rotator().Yaw);
			}

			if (FMath::Abs(DeltaPose.Rotator().Roll) > FMath::Abs(MaxRot.Roll))
			{
				MaxRot.Roll = FMath::Abs(DeltaPose.Rotator().Roll);
			}

			BonePoses.Add(DeltaPose);

			AddCurveKey(CurveTX, Time, 0.f);
			AddCurveKey(CurveTY, Time, 0.f);
			AddCurveKey(CurveTZ, Time, 0.f);
			
			AddCurveKey(CurveRX, Time, 0.f);
			AddCurveKey(CurveRY, Time, 0.f);
			AddCurveKey(CurveRZ, Time, 0.f);

			continue;
		}

		AddCurveKey(CurveTX, Time, DeltaPose.GetLocation().X);
		AddCurveKey(CurveTY, Time, DeltaPose.GetLocation().Y);
		AddCurveKey(CurveTZ, Time, DeltaPose.GetLocation().Z);
			
		AddCurveKey(CurveRX, Time, DeltaPose.Rotator().Roll);
		AddCurveKey(CurveRY, Time, DeltaPose.Rotator().Pitch);
		AddCurveKey(CurveRZ, Time, DeltaPose.Rotator().Yaw);
	}

	if(bNormalize)
	{
		/** Remapping to [0;1] */
		for (int i = 0; i < BonePoses.Num(); i++)
		{
			FVector NormalizedVector;
			NormalizedVector.X = NormalizeValue(BonePoses[i].GetLocation().X, MaxLoc.X);
			NormalizedVector.Y = NormalizeValue(BonePoses[i].GetLocation().Y, MaxLoc.Y);
			NormalizedVector.Z = NormalizeValue(BonePoses[i].GetLocation().Z, MaxLoc.Z);

			FRotator NormalizedRotator;
			NormalizedRotator.Pitch = NormalizeValue(BonePoses[i].Rotator().Pitch, MaxRot.Pitch);
			NormalizedRotator.Yaw = NormalizeValue(BonePoses[i].Rotator().Yaw, MaxRot.Yaw);
			NormalizedRotator.Roll = NormalizeValue(BonePoses[i].Rotator().Roll, MaxRot.Roll);

			BonePoses[i] = FTransform(NormalizedRotator, NormalizedVector);
		}

		for (int i = 0; i < CurveTX.Num(); i++)
		{
			CurveTX[i].Value = BonePoses[i].GetLocation().X;
			CurveTY[i].Value = BonePoses[i].GetLocation().Y;
			CurveTZ[i].Value = BonePoses[i].GetLocation().Z;

			CurveRX[i].Value = BonePoses[i].Rotator().Roll;
			CurveRY[i].Value = BonePoses[i].Rotator().Pitch;
			CurveRZ[i].Value = BonePoses[i].Rotator().Yaw;
		}
	}
	
	AddCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("X")), CurveTX);
	AddCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("Y")), CurveTY);
	AddCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("Z")), CurveTZ);

	AddCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("R")), CurveRX);
	AddCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("P")), CurveRY);
	AddCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("Y")), CurveRZ);
}

void UCurveExtractor::ExtractCurveAssets(UAnimSequence* AnimationSequence)
{
	if (AnimationSequence == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("CurveExtractor failed. Reason: Invalid Animation"));
		return;
	}

	USkeleton* Skeleton = AnimationSequence->GetSkeleton();
	if (Skeleton == nullptr)
	{
		UE_LOG(LogAnimation, Error,
		       TEXT("CurveExtractor failed. Reason: Animation with invalid Skeleton. Animation: %s"),
		       *GetNameSafe(AnimationSequence));
		return;
	}

	const int32 BoneIndex = Skeleton->GetReferenceSkeleton().FindBoneIndex(BoneName);
	if (BoneIndex == INDEX_NONE)
	{
		UE_LOG(LogAnimation, Error,
		       TEXT("CurveExtractor failed. Reason: Invalid Bone Index. BoneName: %s Animation: %s Skeleton: %s"),
		       *BoneName.ToString(), *GetNameSafe(AnimationSequence), *GetNameSafe(Skeleton));
		return;
	}

	FMemMark Mark(FMemStack::Get());

	TArray<FBoneIndexType> RequiredBones;
	RequiredBones.Add(BoneIndex);
	Skeleton->GetReferenceSkeleton().EnsureParentsExistAndSort(RequiredBones);

	FBoneContainer BoneContainer(RequiredBones, false, *Skeleton);
	const FCompactPoseBoneIndex CompactPoseBoneIndex = BoneContainer.
		MakeCompactPoseIndex(FMeshPoseBoneIndex(BoneIndex));

	const float AnimLength = AnimationSequence->GetPlayLength();
	const float SampleInterval = 1.f / SampleRate;

	float Time = 0.f;
	int32 SampleIndex = 0;

	FTransform RefPose = ExtractPose(AnimationSequence, BoneContainer, CompactPoseBoneIndex, 0.f);

	TArray<FTransform> BonePoses;

	FVector MaxLoc = FVector::ZeroVector;
	FRotator MaxRot = FRotator::ZeroRotator;

	auto PluginSettings = GetMutableDefault<UPluginSettings>();

	if (!PluginSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("Plugin Settings Are NULL"))
		return;
	}

	const FString PackagePath = PluginSettings->CurvesSavePath + WeaponName + FString("_Curves/");

	UPackage* PackageLoc = CreatePackage(*(PackagePath + AnimName.ToString() + FString("Loc")));
	PackageLoc->FullyLoad();

	UPackage* PackageRot = CreatePackage(*(PackagePath + AnimName.ToString() + FString("Rot")));
	PackageRot->FullyLoad();

	UCurveVector* TranslationCurve = NewObject<UCurveVector>(PackageLoc, *(AnimName.ToString() + FString("Loc")),
	                                                         RF_Public | RF_Standalone);
	UCurveVector* RotationCurve = NewObject<UCurveVector>(PackageRot, *(AnimName.ToString() + FString("Rot")),
	                                                      RF_Public | RF_Standalone);

	/** Filling poses array with additive data */
	while (Time < AnimLength)
	{
		Time = FMath::Clamp(SampleIndex * SampleInterval, 0.f, AnimLength);
		SampleIndex++;

		FTransform CurrentPose = ExtractPose(AnimationSequence, BoneContainer, CompactPoseBoneIndex, Time);
		FTransform DeltaPose = CurrentPose.Inverse() * RefPose;
		DeltaPose.NormalizeRotation();

		if(bNormalize)
		{
			if (FMath::Abs(DeltaPose.GetLocation().X) > FMath::Abs(MaxLoc.X))
			{
				MaxLoc.X = FMath::Abs(DeltaPose.GetLocation().X);
			}

			if (FMath::Abs(DeltaPose.GetLocation().Y) > FMath::Abs(MaxLoc.Y))
			{
				MaxLoc.Y = FMath::Abs(DeltaPose.GetLocation().Y);
			}

			if (FMath::Abs(DeltaPose.GetLocation().Z) > FMath::Abs(MaxLoc.Z))
			{
				MaxLoc.Z = FMath::Abs(DeltaPose.GetLocation().Z);
			}

			if (FMath::Abs(DeltaPose.Rotator().Pitch) > FMath::Abs(MaxRot.Pitch))
			{
				MaxRot.Pitch = FMath::Abs(DeltaPose.Rotator().Pitch);
			}

			if (FMath::Abs(DeltaPose.Rotator().Yaw) > FMath::Abs(MaxRot.Yaw))
			{
				MaxRot.Yaw = FMath::Abs(DeltaPose.Rotator().Yaw);
			}

			if (FMath::Abs(DeltaPose.Rotator().Roll) > FMath::Abs(MaxRot.Roll))
			{
				MaxRot.Roll = FMath::Abs(DeltaPose.Rotator().Roll);
			}

			BonePoses.Add(DeltaPose);

			TranslationCurve->FloatCurves[0].UpdateOrAddKey(Time, 0.f, true);
			TranslationCurve->FloatCurves[1].UpdateOrAddKey(Time, 0.f, true);
			TranslationCurve->FloatCurves[2].UpdateOrAddKey(Time, 0.f, true);

			RotationCurve->FloatCurves[0].UpdateOrAddKey(Time, 0.f, true);
			RotationCurve->FloatCurves[1].UpdateOrAddKey(Time, 0.f, true);
			RotationCurve->FloatCurves[2].UpdateOrAddKey(Time, 0.f, true);
			
			continue;
		}
		
		TranslationCurve->FloatCurves[0].UpdateOrAddKey(Time, DeltaPose.GetLocation().X, true);
		TranslationCurve->FloatCurves[1].UpdateOrAddKey(Time, DeltaPose.GetLocation().Y, true);
		TranslationCurve->FloatCurves[2].UpdateOrAddKey(Time, DeltaPose.GetLocation().Z, true);

		RotationCurve->FloatCurves[0].UpdateOrAddKey(Time, DeltaPose.Rotator().Roll, true);
		RotationCurve->FloatCurves[1].UpdateOrAddKey(Time, DeltaPose.Rotator().Pitch, true);
		RotationCurve->FloatCurves[2].UpdateOrAddKey(Time, DeltaPose.Rotator().Yaw, true);
	}

	if(bNormalize)
	{
		/** Remapping to [0;1] */
		for (int i = 0; i < BonePoses.Num(); i++)
		{
			FVector NormalizedVector;
			NormalizedVector.X = NormalizeValue(BonePoses[i].GetLocation().X, MaxLoc.X);
			NormalizedVector.Y = NormalizeValue(BonePoses[i].GetLocation().Y, MaxLoc.Y);
			NormalizedVector.Z = NormalizeValue(BonePoses[i].GetLocation().Z, MaxLoc.Z);

			FRotator NormalizedRotator;
			NormalizedRotator.Pitch = NormalizeValue(BonePoses[i].Rotator().Pitch, MaxRot.Pitch);
			NormalizedRotator.Yaw = NormalizeValue(BonePoses[i].Rotator().Yaw, MaxRot.Yaw);
			NormalizedRotator.Roll = NormalizeValue(BonePoses[i].Rotator().Roll, MaxRot.Roll);

			BonePoses[i] = FTransform(NormalizedRotator, NormalizedVector);
		}

		for (int i = 0; i < TranslationCurve->FloatCurves->GetNumKeys(); i++)
		{
			TranslationCurve->FloatCurves[0].Keys[i].Value = BonePoses[i].GetLocation().X;
			TranslationCurve->FloatCurves[1].Keys[i].Value = BonePoses[i].GetLocation().Y;
			TranslationCurve->FloatCurves[2].Keys[i].Value = BonePoses[i].GetLocation().Z;

			RotationCurve->FloatCurves[0].Keys[i].Value = BonePoses[i].Rotator().Roll;
			RotationCurve->FloatCurves[1].Keys[i].Value = BonePoses[i].Rotator().Pitch;
			RotationCurve->FloatCurves[2].Keys[i].Value = BonePoses[i].Rotator().Yaw;
		}
	}
	
	SaveCurve(PackageLoc, TranslationCurve, PackagePath + AnimName.ToString() + FString("Loc"));
	SaveCurve(PackageRot, RotationCurve, PackagePath + AnimName.ToString() + FString("Rot"));
}

void UCurveExtractor::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	Super::OnApply_Implementation(AnimationSequence);

	if(bAddToAnimation)
	{
		ExtractAnimCurves(AnimationSequence);
		return;
	}

	ExtractCurveAssets(AnimationSequence);
}

void UCurveExtractor::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	Super::OnRevert_Implementation(AnimationSequence);

	RemoveCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("X")));
	RemoveCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("Y")));
	RemoveCurve(AnimationSequence, GetCurveName(TEXT("T"), TEXT("Z")));

	RemoveCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("R")));
	RemoveCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("P")));
	RemoveCurve(AnimationSequence, GetCurveName(TEXT("R"), TEXT("Y")));
}

FTransform UCurveExtractor::ExtractPose(UAnimSequence* Animation, const FBoneContainer& BoneContainer,
                                                 FCompactPoseBoneIndex CompactPoseBoneIndex, double Time)
{
	FCompactPose Pose;
	Pose.SetBoneContainer(&BoneContainer);

	FBlendedCurve Curve;
	Curve.InitFrom(BoneContainer);

	FAnimExtractContext Context(Time, false);
	UE::Anim::FStackAttributeContainer Attributes;
	FAnimationPoseData AnimationPoseData(Pose, Curve, Attributes);

	Animation->GetBonePose(AnimationPoseData, Context, true);

	check(Pose.IsValidIndex(CompactPoseBoneIndex));
	
	FCSPose<FCompactPose> ComponentSpacePose;
	ComponentSpacePose.InitPose(Pose);
	return ComponentSpacePose.GetComponentSpaceTransform(CompactPoseBoneIndex);
}

void UCurveExtractor::SaveCurve(UPackage* Package, UCurveVector* Curve, const FString& PackagePath)
{
	FAssetRegistryModule::AssetCreated(Curve);

	/** Getting the full path to the file */
	const FString FilePath = FPackageName::LongPackageNameToFilename(PackagePath,
																	 FPackageName::GetAssetPackageExtension());

	/** Finally saving just created package */
	FSavePackageArgs SavePackageArgs;
	SavePackageArgs.TopLevelFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
	const bool bSuccess = UPackage::SavePackage(Package, Curve, *FilePath, SavePackageArgs);
	
	Package->SetDirtyFlag(true);

	UE_LOG(LogTemp, Warning, TEXT("Saved Recoil Curves: %s"), bSuccess ? TEXT("SUCCESS") : TEXT("ERROR"))
}