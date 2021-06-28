// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimNode_CopyPoseFromPoseableMesh.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimTrace.h"

/////////////////////////////////////////////////////
// FAnimNode_CopyPoseFromPoseableMesh

FAnimNode_CopyPoseFromPoseableMesh::FAnimNode_CopyPoseFromPoseableMesh()
	: SourceMeshComponent(nullptr)
{
}

void FAnimNode_CopyPoseFromPoseableMesh::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	FAnimNode_Base::Initialize_AnyThread(Context);

	// Initial update of the node, so we dont have a frame-delay on setup
	GetEvaluateGraphExposedInputs().Execute(Context);
	RefreshMeshComponent(Context.AnimInstanceProxy->GetSkelMeshComponent());
}

void FAnimNode_CopyPoseFromPoseableMesh::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)

}

void FAnimNode_CopyPoseFromPoseableMesh::RefreshMeshComponent(USkinnedMeshComponent* TargetMeshComponent)
{
	auto ResetMeshComponent = [this](USkinnedMeshComponent* InMeshComponent, USkinnedMeshComponent* InTargetMeshComponent)
	{
		USkinnedMeshComponent* CurrentMeshComponent = CurrentlyUsedSourceMeshComponent.Get();
		// if current mesh exists, but not same as input mesh
		if (CurrentMeshComponent)
		{
			// if component has been changed, reinitialize
			if (CurrentMeshComponent != InMeshComponent)
			{
				ReinitializeMeshComponent(InMeshComponent, InTargetMeshComponent);
			}
			// if component is still same but mesh has been changed, we have to reinitialize
			else if (CurrentMeshComponent->SkeletalMesh != CurrentlyUsedSourceMesh.Get())
			{
				ReinitializeMeshComponent(InMeshComponent, InTargetMeshComponent);
			}
			else if (InTargetMeshComponent)
			{
				// see if target mesh has changed
				if (InTargetMeshComponent->SkeletalMesh != CurrentlyUsedTargetMesh.Get())
				{
					ReinitializeMeshComponent(InMeshComponent, InTargetMeshComponent);
				}
			}
			else if (CurrentTargetFromSourceBoneOptions.Num() != TargetBoneFromSourceBoneOptions.Num()) {
				ReinitializeMeshComponent(InMeshComponent, InTargetMeshComponent);
			}
		}
		// if not valid, but input mesh is
		else if (!CurrentMeshComponent && InMeshComponent)
		{
			ReinitializeMeshComponent(InMeshComponent, InTargetMeshComponent);
		}
	};

	if(SourceMeshComponent.IsValid())
	{
		ResetMeshComponent(SourceMeshComponent.Get(), TargetMeshComponent);
	}
	else
	{
		CurrentlyUsedSourceMeshComponent.Reset();
	}
}

void FAnimNode_CopyPoseFromPoseableMesh::PreUpdate(const UAnimInstance* InAnimInstance)
{
	QUICK_SCOPE_CYCLE_COUNTER(FAnimNode_CopyPoseFromPoseableMesh_PreUpdate);

	USkinnedMeshComponent* CurrentMeshComponent = CurrentlyUsedSourceMeshComponent.IsValid() ? CurrentlyUsedSourceMeshComponent.Get() : nullptr;

	if (CurrentMeshComponent && CurrentMeshComponent->SkeletalMesh && CurrentMeshComponent->IsRegistered())
	{
		//// If our source is running under master-pose, then get bone data from there
		//if(USkeletalMeshComponent* MasterPoseComponent = Cast<USkeletalMeshComponent>(CurrentMeshComponent->MasterPoseComponent.Get()))
		//{
		//	CurrentMeshComponent = MasterPoseComponent;
		//}

		// re-check mesh component validity as it may have changed to master
		if(CurrentMeshComponent->SkeletalMesh && CurrentMeshComponent->IsRegistered())
		{
			const bool bUROInSync = CurrentMeshComponent->ShouldUseUpdateRateOptimizations() && CurrentMeshComponent->AnimUpdateRateParams != nullptr && CurrentMeshComponent->AnimUpdateRateParams == InAnimInstance->GetSkelMeshComponent()->AnimUpdateRateParams;
			const bool bUsingExternalInterpolation = CurrentMeshComponent->IsUsingExternalInterpolation();
			const TArray<FTransform>& CachedComponentSpaceTransforms = CurrentMeshComponent->GetComponentSpaceTransforms();
			const bool bArraySizesMatch = CachedComponentSpaceTransforms.Num() == CurrentMeshComponent->GetComponentSpaceTransforms().Num();

			// Copy source array from the appropriate location
			SourceMeshTransformArray.Reset();
			

			SourceMeshTransformArray.Append((bUROInSync || bUsingExternalInterpolation) && bArraySizesMatch ? CachedComponentSpaceTransforms : CurrentMeshComponent->GetComponentSpaceTransforms());

			// Ref skeleton is need for parent index lookups later, so store it now
			CurrentlyUsedMesh = CurrentMeshComponent->SkeletalMesh;
		}
		else
		{
			CurrentlyUsedMesh.Reset();
		}
	}
}

void FAnimNode_CopyPoseFromPoseableMesh::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread)
	// This introduces a frame of latency in setting the pin-driven source component,
	// but we cannot do the work to extract transforms on a worker thread as it is not thread safe.
	GetEvaluateGraphExposedInputs().Execute(Context);
	RefreshMeshComponent(Context.AnimInstanceProxy->GetSkelMeshComponent());

	TRACE_ANIM_NODE_VALUE(Context, TEXT("Component"), *GetNameSafe(CurrentlyUsedSourceMeshComponent.IsValid() ? CurrentlyUsedSourceMeshComponent.Get() : nullptr));
	TRACE_ANIM_NODE_VALUE(Context, TEXT("Mesh"), *GetNameSafe(CurrentlyUsedSourceMeshComponent.IsValid() ? CurrentlyUsedSourceMeshComponent.Get()->SkeletalMesh : nullptr));
}

void FAnimNode_CopyPoseFromPoseableMesh::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Evaluate_AnyThread)
	FCompactPose& OutPose = Output.Pose;
	OutPose.ResetToRefPose();
	USkeletalMesh* CurrentMesh = CurrentlyUsedMesh.IsValid() ? CurrentlyUsedMesh.Get() : nullptr;

	if(SourceMeshTransformArray.Num() > 0 && CurrentMesh)
	{
		const FBoneContainer& RequiredBones = OutPose.GetBoneContainer();

		for(FCompactPoseBoneIndex PoseBoneIndex : OutPose.ForEachBoneIndex())
		{
			const int32 SkeletonBoneIndex = RequiredBones.GetSkeletonIndex(PoseBoneIndex);
			const int32 MeshBoneIndex = RequiredBones.GetSkeletonToPoseBoneIndexArray()[SkeletonBoneIndex];
			const FCopyPoseBoneOptions* BoneOptions = BoneMapToSource.Find(MeshBoneIndex);

			if (BoneOptions) {
				if (SourceMeshTransformArray.IsValidIndex(BoneOptions->SourceBoneIndex))
				{
					const int32 SourceBoneIndex = BoneOptions->SourceBoneIndex;
					
					FTransform OutTransform = OutPose[PoseBoneIndex];
					FTransform SourceTransform;
					ECopyTransformSpace TransformSpace = (BoneOptions->OverrideCopyTransformSpace != ECopyTransformSpace::None) ? BoneOptions->OverrideCopyTransformSpace : CopyTransformSpace;
					const int32 ParentIndex = (BoneOptions->ParentBoneIndex >= 0) ? BoneOptions->ParentBoneIndex : CurrentMesh->RefSkeleton.GetParentIndex(SourceBoneIndex);

					const FCompactPoseBoneIndex MyParentIndex = RequiredBones.GetParentBoneIndex(PoseBoneIndex);

					switch (TransformSpace) {
					case ECopyTransformSpace::CopyInParentSpace:
					{
						// only apply if I also have parent, otherwise, it should apply the space bases
						if (SourceMeshTransformArray.IsValidIndex(ParentIndex) && MyParentIndex != INDEX_NONE)
						{
							const FTransform& ParentTransform = SourceMeshTransformArray[ParentIndex];
							const FTransform& ChildTransform = SourceMeshTransformArray[SourceBoneIndex];
							FTransform RelativeTransform = ChildTransform.GetRelativeTransform(ParentTransform);
							SourceTransform = RelativeTransform;
						}
						else
						{
							SourceTransform = SourceMeshTransformArray[SourceBoneIndex];
						}
						break;
					}
					case ECopyTransformSpace::CopyInLocalSpace:
						SourceTransform = SourceMeshTransformArray[SourceBoneIndex];
						break;
					case ECopyTransformSpace::CopyInComponentSpace:
						SourceTransform = SourceMeshTransformArray[SourceBoneIndex];
						break;
					case ECopyTransformSpace::CopyInInWorldSpace:
					{
						FTransform SourceWorldTransform = CurrentlyUsedSourceMeshComponent->GetComponentTransform() + SourceMeshTransformArray[SourceBoneIndex];

						FVector loc;
						FRotator rot;
						Output.AnimInstanceProxy->GetSkelMeshComponent()->TransformToBoneSpace(
							Output.AnimInstanceProxy->GetSkelMeshComponent()->SkeletalMesh->RefSkeleton.GetBoneName(PoseBoneIndex.GetInt()), 
							SourceWorldTransform.GetLocation(), 
							SourceWorldTransform.GetRotation().Rotator(), 
							loc, 
							rot
						);

						SourceTransform.SetLocation(loc);
						SourceTransform.SetRotation(rot.Quaternion());
						break;
					}
					default:
						break;
					}

					if (BoneOptions->CopyableTransformsFlags & static_cast<uint8>(ECopyableTransforms::kCopyRotation)) {
						OutTransform.SetRotation(SourceTransform.GetRotation());
					}
					if (BoneOptions->CopyableTransformsFlags & static_cast<uint8>(ECopyableTransforms::kCopyTranslation)) {
						OutTransform.SetTranslation(SourceTransform.GetTranslation());
					}
					if (BoneOptions->CopyableTransformsFlags & static_cast<uint8>(ECopyableTransforms::kCopyScale)) {
						OutTransform.SetScale3D(SourceTransform.GetScale3D());
					}

					OutPose[PoseBoneIndex] = OutTransform;
				}
			}
		}
	}
}

void FAnimNode_CopyPoseFromPoseableMesh::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	FString DebugLine = DebugData.GetNodeName(this);

	DebugLine += FString::Printf(TEXT("('%s')"), *GetNameSafe(CurrentlyUsedSourceMeshComponent.IsValid() ? CurrentlyUsedSourceMeshComponent.Get()->SkeletalMesh : nullptr));
	
	USkinnedMeshComponent* CurrentMeshComponent = CurrentlyUsedSourceMeshComponent.IsValid() ? CurrentlyUsedSourceMeshComponent.Get() : nullptr;
	if (CurrentMeshComponent) {
		FString FingerData = TEXT("Hand rotations: ");
		for (size_t index = 0; index < CurrentMeshComponent->GetComponentSpaceTransforms().Num(); ++index) {
			//FingerData += FString::Printf(TEXT("  %s\n"), *CurrentMeshComponent->GetComponentSpaceTransforms()[index].GetRotation().ToString());
		}
		DebugData.AddDebugItem(FingerData, true);
	}
}

void FAnimNode_CopyPoseFromPoseableMesh::ReinitializeMeshComponent(USkinnedMeshComponent* NewSourceMeshComponent, USkinnedMeshComponent* TargetMeshComponent)
{
	CurrentlyUsedSourceMeshComponent.Reset();
	// reset source mesh
	CurrentlyUsedSourceMesh.Reset();
	CurrentlyUsedTargetMesh.Reset();
	BoneMapToSource.Reset();
	CurveNameToUIDMap.Reset();
	CurrentTargetFromSourceBoneOptions = TargetBoneFromSourceBoneOptions;

	if (TargetMeshComponent && NewSourceMeshComponent && NewSourceMeshComponent->SkeletalMesh && !NewSourceMeshComponent->IsPendingKill())
	{
		USkeletalMesh* SourceSkelMesh = NewSourceMeshComponent->SkeletalMesh;
		USkeletalMesh* TargetSkelMesh = TargetMeshComponent->SkeletalMesh;
		
		if (SourceSkelMesh && !SourceSkelMesh->IsPendingKill() && !SourceSkelMesh->HasAnyFlags(RF_NeedPostLoad) &&
			TargetSkelMesh && !TargetSkelMesh->IsPendingKill() && !TargetSkelMesh->HasAnyFlags(RF_NeedPostLoad))
		{
			CurrentlyUsedSourceMeshComponent = NewSourceMeshComponent;
			CurrentlyUsedSourceMesh = SourceSkelMesh;
			CurrentlyUsedTargetMesh = TargetSkelMesh;

			ECopyableTransforms DefaultCopyableTransformFlags;
			DefaultCopyableTransformFlags |= ECopyableTransforms::kCopyTranslation;
			DefaultCopyableTransformFlags |= ECopyableTransforms::kCopyRotation;
			DefaultCopyableTransformFlags |= ECopyableTransforms::kCopyScale;

			FName BoneName_Target;
			int32 ParentIndex = -1;

			if (SourceSkelMesh == TargetSkelMesh)
			{
				for(int32 ComponentSpaceBoneId = 0; ComponentSpaceBoneId < SourceSkelMesh->RefSkeleton.GetNum(); ++ComponentSpaceBoneId)
				{
					BoneName_Target = TargetSkelMesh->RefSkeleton.GetBoneName(ComponentSpaceBoneId);
					int32 SourceBoneIndex = SourceSkelMesh->RefSkeleton.FindBoneIndex(BoneName_Target);

					FCopyPoseBoneOptions boneoptions{ 
						TargetSkelMesh->RefSkeleton.GetBoneName(ComponentSpaceBoneId),
						static_cast<uint8>(DefaultCopyableTransformFlags), 
						ECopyTransformSpace::None,
						NAME_None,
						SourceBoneIndex,
						ParentIndex
					};
					BoneMapToSource.Add(ComponentSpaceBoneId, boneoptions);
				}
			}
			else
			{
				for (int32 ComponentSpaceBoneId=0; ComponentSpaceBoneId<TargetSkelMesh->RefSkeleton.GetNum(); ++ComponentSpaceBoneId)
				{
					BoneName_Target = TargetSkelMesh->RefSkeleton.GetBoneName(ComponentSpaceBoneId);
					int32 SourceBoneIndex = SourceSkelMesh->RefSkeleton.FindBoneIndex(BoneName_Target);

					FCopyPoseBoneOptions* BoneCopyOptions = CurrentTargetFromSourceBoneOptions.Find(BoneName_Target);
					if (BoneCopyOptions) {
						BoneCopyOptions->ParentBoneIndex = (!BoneCopyOptions->OverrideSourceBoneParent.IsNone()) ? SourceSkelMesh->RefSkeleton.FindBoneIndex(BoneCopyOptions->OverrideSourceBoneParent) : ParentIndex;
						BoneCopyOptions->SourceBoneIndex = SourceSkelMesh->RefSkeleton.FindBoneIndex(BoneCopyOptions->SourceBone);
						BoneMapToSource.Add(ComponentSpaceBoneId, *BoneCopyOptions);
					}
					else {
						// No bone found
						FCopyPoseBoneOptions boneoptions{ 
							TargetSkelMesh->RefSkeleton.GetBoneName(ComponentSpaceBoneId), 
							static_cast<uint8>(DefaultCopyableTransformFlags),
							ECopyTransformSpace::None,
							NAME_None,
							SourceSkelMesh->RefSkeleton.FindBoneIndex(BoneName_Target),
							ParentIndex
						};
						BoneMapToSource.Add(ComponentSpaceBoneId, boneoptions);
					}
				}
			}
		}
	}
}
