// Designed by KINEMATION, 2025. All rights reserved.

#include "AnimNode_RecoilAnimationNode.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "RecoilAnimationComponent.h"

FTransform FAnimNode_RecoilAnimationNode::ComputeRecoilTransform()
{
	FTransform FinalTransform = bUseAutoUpdate
		                            ? RecoilTransform
		                            : FTransform(Rotation.Quaternion(), Translation);

	switch (AxesOrder)
	{
	case ERecoilAxesOrder::XZY:
		FinalTransform.SetLocation(FVector(FinalTransform.GetLocation().X, FinalTransform.GetLocation().Z,
		                                   FinalTransform.GetLocation().Y));
		FinalTransform.SetRotation(FRotator(FinalTransform.Rotator().Pitch, FinalTransform.Rotator().Roll,
		                                    FinalTransform.Rotator().Yaw).Quaternion());
		break;
	case ERecoilAxesOrder::YXZ:
		FinalTransform.SetLocation(FVector(FinalTransform.GetLocation().Y, FinalTransform.GetLocation().X,
		                                   FinalTransform.GetLocation().Z));
		FinalTransform.SetRotation(FRotator(FinalTransform.Rotator().Roll, FinalTransform.Rotator().Yaw,
		                                    FinalTransform.Rotator().Pitch).Quaternion());
		break;
	case ERecoilAxesOrder::YZX:
		FinalTransform.SetLocation(FVector(FinalTransform.GetLocation().Y, FinalTransform.GetLocation().Z,
		                                   FinalTransform.GetLocation().X));
		FinalTransform.SetRotation(FRotator(FinalTransform.Rotator().Roll, FinalTransform.Rotator().Pitch,
		                                    FinalTransform.Rotator().Yaw).Quaternion());
		break;
	case ERecoilAxesOrder::ZXY:
		FinalTransform.SetLocation(FVector(FinalTransform.GetLocation().Z, FinalTransform.GetLocation().X,
		                                   FinalTransform.GetLocation().Y));
		FinalTransform.SetRotation(FRotator(FinalTransform.Rotator().Yaw, FinalTransform.Rotator().Pitch,
		                                    FinalTransform.Rotator().Roll).Quaternion());
		break;
	case ERecoilAxesOrder::ZYX:
		FinalTransform.SetLocation(FVector(FinalTransform.GetLocation().Z, FinalTransform.GetLocation().Y,
		                                   FinalTransform.GetLocation().X));
		FinalTransform.SetRotation(FRotator(FinalTransform.Rotator().Yaw, FinalTransform.Rotator().Roll,
		                                    FinalTransform.Rotator().Pitch).Quaternion());
		break;
	default:
		break;
	}

	return FinalTransform;
}

void FAnimNode_RecoilAnimationNode::PreUpdate(const UAnimInstance* InAnimInstance)
{
	if (!bIsInitialized)
	{
		if (auto Owner = InAnimInstance->GetOwningActor())
		{
			if (!Owner->HasActorBegunPlay())
			{
				return;
			}

			if (auto Component = Owner->FindComponentByClass<URecoilAnimationComponent>())
			{
				RecoilComponent = Component;
			}
		}

		bIsInitialized = true;
	}
	if (RecoilComponent.IsValid())
	{
		RecoilTransform = RecoilComponent->GetOutput();
	}
}

void FAnimNode_RecoilAnimationNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
                                                                      TArray<FBoneTransform>& OutBoneTransforms)
{
	check(OutBoneTransforms.Num() == 0);

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	FCompactPoseBoneIndex CompactPoseBoneToModify = BoneToModify.GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = Output.Pose.GetComponentSpaceTransform(CompactPoseBoneToModify);
	FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, NewBoneTM,
	                                                 CompactPoseBoneToModify, RotationSpace);

	FTransform FinalTransform = ComputeRecoilTransform();

	FVector FinalPosition = FinalTransform.GetTranslation();
	FRotator FinalRotation = FinalTransform.Rotator();

	FinalPosition.X *= TranslationScale.X;
	FinalPosition.Y *= TranslationScale.Y;
	FinalPosition.Z *= TranslationScale.Z;

	FinalRotation.Pitch *= RotationScale.Y;
	FinalRotation.Yaw *= RotationScale.Z;
	FinalRotation.Roll *= RotationScale.X;

	FinalTransform.SetTranslation(FinalPosition);
	FinalTransform.SetRotation(FinalRotation.Quaternion());

	const FQuat BoneQuat = FinalTransform.GetRotation();

	NewBoneTM.SetRotation(BoneQuat * NewBoneTM.GetRotation());

	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, NewBoneTM,
	                                                 CompactPoseBoneToModify, RotationSpace);

	FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, NewBoneTM,
	                                                 CompactPoseBoneToModify, TranslationSpace);

	NewBoneTM.AddToTranslation(FinalTransform.GetTranslation());


	FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, NewBoneTM,
	                                                 CompactPoseBoneToModify, TranslationSpace);


	OutBoneTransforms.Add(FBoneTransform(BoneToModify.GetCompactPoseIndex(BoneContainer), NewBoneTM));
}

bool FAnimNode_RecoilAnimationNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return (BoneToModify.IsValidToEvaluate(RequiredBones));
}

void FAnimNode_RecoilAnimationNode::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(InitializeBoneReferences)
	BoneToModify.Initialize(RequiredBones);
}
