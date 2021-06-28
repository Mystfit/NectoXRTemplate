// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimGraphNode_CopyPoseFromPoseableMesh.h"

/////////////////////////////////////////////////////
// UAnimGraphNode_CopyPoseFromPoseableMeshSkeletalControl

#define LOCTEXT_NAMESPACE "A3Nodes"

UAnimGraphNode_CopyPoseFromPoseableMesh::UAnimGraphNode_CopyPoseFromPoseableMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FText UAnimGraphNode_CopyPoseFromPoseableMesh::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_CopyPoseFromMesh_Tooltip", "The Copy Pose From Poseable Mesh node copies the pose data from a POseable Mesh Component component to this. Only works when name matches.");
}

FText UAnimGraphNode_CopyPoseFromPoseableMesh::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("CopyPoseFromPoseableMesh", "Copy Pose From Poseable Mesh");
}

#undef LOCTEXT_NAMESPACE
