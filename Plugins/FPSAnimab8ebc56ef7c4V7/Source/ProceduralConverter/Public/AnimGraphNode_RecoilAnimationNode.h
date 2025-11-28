// Designed by KINEMATION, 2025. All rights reserved.

#pragma once

#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNode_RecoilAnimationNode.h"
#include "AnimGraphNode_RecoilAnimationNode.generated.h"

UCLASS(meta=(Keywords = "Recoil Animation"))
class PROCEDURALCONVERTER_API UAnimGraphNode_RecoilAnimationNode : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Settings)
	FAnimNode_RecoilAnimationNode Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

protected:
	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	// End of UAnimGraphNode_SkeletalControlBase interface

};