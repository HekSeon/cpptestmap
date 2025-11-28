// Designed by KINEMATION, 2025. All rights reserved.

#include "AnimGraphNode_RecoilAnimationNode.h"

FText UAnimGraphNode_RecoilAnimationNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Recoil Animation"));
}

FText UAnimGraphNode_RecoilAnimationNode::GetTooltipText() const
{
	return FText::FromString(TEXT("Applies weapon recoil to the specified bone."));
}

FText UAnimGraphNode_RecoilAnimationNode::GetControllerDescription() const
{
	return FText::FromString(TEXT("Recoil Animation Node"));
}