// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AnimGraphNode_Base.h"
#include "AnimNode_CopyPoseFromPoseableMesh.h"
#include "AnimGraphNode_CopyPoseFromPoseableMesh.generated.h"

UCLASS(MinimalAPI)
class UAnimGraphNode_CopyPoseFromPoseableMesh : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category=Settings)
	FAnimNode_CopyPoseFromPoseableMesh Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

	// UAnimGraphNode_Base interface
	virtual bool UsingCopyPoseFromPoseableMesh() { return true; }
	// End of UAnimGraphNode_Base interface
};
