// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNodeBase.h"
#include "Components/PoseableMeshComponent.h"
#include "AnimNode_CopyPoseFromPoseableMesh.generated.h"

class USkeletalMeshComponent;
struct FAnimInstanceProxy;


UENUM(BlueprintType, meta = (Bitflags))
enum class ECopyableTransforms : uint8
{
	None = 0 UMETA(Hidden),
	kCopyTranslation = 1 << 0 UMETA(DisplayName = "Copy translation"),
	kCopyRotation = 1 << 1 UMETA(DisplayName = "Copy rotation"),
	kCopyScale = 1 << 2 UMETA(DisplayName = "Copy scale")
};
ENUM_CLASS_FLAGS(ECopyableTransforms)


UENUM(BlueprintType)
enum class ECopyTransformSpace : uint8
{
	None = 0 UMETA(Hidden),
	CopyInParentSpace UMETA(DisplayName = "Copy in parent space"),
	CopyInLocalSpace UMETA(DisplayName = "Copy in local space"),
	CopyInComponentSpace UMETA(DisplayName = "Copy in component space"),
	CopyInInWorldSpace UMETA(DisplayName = "Copy in world space")
};
ENUM_CLASS_FLAGS(ECopyTransformSpace)




USTRUCT(Blueprintable)
struct FCopyPoseBoneOptions {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SourceBone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (Bitmask, BitmaskEnum = "ECopyableTransforms"))
	uint8 CopyableTransformsFlags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ECopyTransformSpace OverrideCopyTransformSpace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName OverrideSourceBoneParent;

	int32 SourceBoneIndex;

	int32 ParentBoneIndex;
};



/**
 *	Copy 
 */



USTRUCT(BlueprintInternalUseOnly)
struct HANDPOSES_API FAnimNode_CopyPoseFromPoseableMesh : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	/*  This is used by default if it's valid */
	UPROPERTY(BlueprintReadWrite, transient, Category=Copy, meta=(PinShownByDefault))
	TWeakObjectPtr<USkinnedMeshComponent> SourceMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Copy, meta = (PinShownByDefault))
	mutable TMap<FName, FCopyPoseBoneOptions> TargetBoneFromSourceBoneOptions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Copy, meta = (PinShownByDefault))
	ECopyTransformSpace CopyTransformSpace = ECopyTransformSpace::CopyInParentSpace;

	FAnimNode_CopyPoseFromPoseableMesh();

	// FAnimNode_Base interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	virtual bool HasPreUpdate() const override { return true; }
	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;
	// End of FAnimNode_Base interface

private:
	// this is source mesh references, so that we could compare and see if it has changed
	TWeakObjectPtr<USkinnedMeshComponent>	CurrentlyUsedSourceMeshComponent;
	TWeakObjectPtr<USkeletalMesh>			CurrentlyUsedSourceMesh;
	TWeakObjectPtr<USkeletalMesh>			CurrentlyUsedMesh;

	// target mesh 
	TWeakObjectPtr<USkeletalMesh>			CurrentlyUsedTargetMesh;
	// cache of target space bases to source space bases
	TMap<int32, FCopyPoseBoneOptions> BoneMapToSource;
	//cache of target space bases to bony copy options
	//TMap<int32, FCopyPoseBoneOptions> BoneMapToSourceCopyOptions;

	TMap<FName, SmartName::UID_Type> CurveNameToUIDMap;

	// Cached bonename map
	TMap<FName, FCopyPoseBoneOptions> CurrentTargetFromSourceBoneOptions;

	// Cached transforms, copied on the game thread
	TArray<FTransform> SourceMeshTransformArray;

	// Cached curves, copied on the game thread
	TMap<FName, float> SourceCurveList;

	// reinitialize mesh component 
	void ReinitializeMeshComponent(USkinnedMeshComponent* NewSourceMeshComponent, USkinnedMeshComponent* TargetMeshComponent);
	void RefreshMeshComponent(USkinnedMeshComponent* TargetMeshComponent);
};
