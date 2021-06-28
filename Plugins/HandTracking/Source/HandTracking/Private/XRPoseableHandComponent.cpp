// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPoseableHandComponent.h"
#include "IXRTrackingSystem.h"
#include "XRMotionControllerBase.h"
#include "XRTrackingSystemBase.h"
#include "UObject/Class.h"
#include "HeadMountedDisplayFunctionLibrary.h"

UXRPoseableHandComponent::UXRPoseableHandComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bTickEvenWhenPaused = true;

	BoneNameMapping = {
		{EHandKeypoint::Wrist, "Wrist Root"},
		{EHandKeypoint::Palm, "Palm"},
		{EHandKeypoint::ThumbMetacarpal, "Thumb1"},
		{EHandKeypoint::ThumbProximal, "Thumb2"},
		{EHandKeypoint::ThumbDistal, "Thumb3"},
		{EHandKeypoint::ThumbTip, "Thumb Tip"},
		{EHandKeypoint::IndexMetacarpal, "Index0"},
		{EHandKeypoint::IndexProximal, "Index1"},
		{EHandKeypoint::IndexIntermediate, "Index2"},
		{EHandKeypoint::IndexDistal, "Index3"},
		{EHandKeypoint::IndexTip, "Index Tip"},
		{EHandKeypoint::MiddleMetacarpal, "Middle0"},
		{EHandKeypoint::MiddleProximal, "Middle1"},
		{EHandKeypoint::MiddleIntermediate, "Middle2"},
		{EHandKeypoint::MiddleDistal, "Middle3"},
		{EHandKeypoint::MiddleTip, "Middle Tip"},
		{EHandKeypoint::RingMetacarpal, "Ring0"},
		{EHandKeypoint::RingProximal, "Ring1"},
		{EHandKeypoint::RingIntermediate, "Ring2"},
		{EHandKeypoint::RingDistal, "Ring3"},
		{EHandKeypoint::RingTip, "Ring Tip"},
		{EHandKeypoint::LittleMetacarpal, "Pinky0"},
		{EHandKeypoint::LittleProximal, "Pinky1"},
		{EHandKeypoint::LittleIntermediate, "Pinky2"},
		{EHandKeypoint::LittleDistal, "Pinky3"},
		{EHandKeypoint::LittleTip, "Pinky Tip"}
	};
}


void UXRPoseableHandComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsInGameThread())
	{
		// Cache state from the game thread for use on the render thread
		const AActor* MyOwner = GetOwner();
		bHasAuthority = MyOwner->HasLocalNetOwner();
	}

	if (bHasAuthority)
	{
		bool bHidden = false;
		UpdateBonePose();
	}
}


EControllerHand UXRPoseableHandComponent::TrackedControllerType() const
{
	if (MotionControllerSource) {
		EControllerHand Hand;
		FXRMotionControllerBase::GetHandEnumForSourceName(MotionControllerSource->MotionSource, Hand);
		return Hand;
	}
	return EControllerHand::AnyHand;
}

TMap<EHandKeypoint, FTransform> UXRPoseableHandComponent::GetAllBoneTransforms() const
{
	TMap<EHandKeypoint, FTransform> transforms;
	for (int index = 0; index < EHandKeypointCount; ++index) {
		EHandKeypoint key = static_cast<EHandKeypoint>(index);
		auto name = BoneNameMapping.Find(key);
		if (name) {
			transforms[key] = BoneSpaceTransforms[index];
		}
	}
	return transforms;
}

void UXRPoseableHandComponent::UpdateBonePose()
{
	if (MotionControllerSource) {
		FXRMotionControllerData MotionControllerData;
		EControllerHand Hand = TrackedControllerType();
		if (Hand == EControllerHand::Left || Hand == EControllerHand::Right) {
			UHeadMountedDisplayFunctionLibrary::GetMotionControllerData(this->GetWorld(), Hand, MotionControllerData);
			if (MotionControllerData.DeviceVisualType == EXRVisualType::Hand) {
				if (MotionControllerData.HandKeyRotations.Num()) {
					for (int index = 0; index < EHandKeypointCount; ++index) {
						FName* name = BoneNameMapping.Find(static_cast<EHandKeypoint>(index));
						if (name) {
							FTransform FingerTransform(MotionControllerData.HandKeyRotations[index], MotionControllerData.HandKeyPositions[index], FVector::OneVector); //MotionControllerData.HandKeyRadii[index])
							SetBoneTransformByName(*name, FingerTransform, EBoneSpaces::ComponentSpace);
						}
						else {
							UE_LOG(LogTemp, Log, TEXT("No target bone found for %i"), *UEnum::GetValueAsString<EHandKeypoint>(static_cast<EHandKeypoint>(index)));
						}
					}
				}
				else {
					//UE_LOG(LogTemp, Log, TEXT("No HandKeyRotations found"));
				}
			}
			//
			//for (auto rotation : MotionControllerData.HandKeyRotations) {
			//	UE_LOG(LogTemp, Log, TEXT("Hand: %s Rotation: %s"), *MotionControllerSource->MotionSource.ToString(), *rotation.ToString());
			//}
		}
	}

	//// Set Root Bone Rotation
	//FQuat RootBoneRotation = UOculusInputFunctionLibrary::GetBoneRotation(SkeletonType, EBone::Wrist_Root);
	//RootBoneRotation *= HandRootFixupRotation;
	//RootBoneRotation.Normalize();
	//BoneSpaceTransforms[0].SetRotation(RootBoneRotation);

	//// Set Remaining Bone Rotations
	//for (uint32 BoneIndex = 1; BoneIndex < (uint32)SkeletalMesh->RefSkeleton.GetNum(); BoneIndex++)
	//{
	//	FQuat BoneRotation = UOculusInputFunctionLibrary::GetBoneRotation(SkeletonType, (EBone)BoneIndex);
	//	BoneSpaceTransforms[BoneIndex].SetRotation(BoneRotation);
	//}
	//
	MarkRefreshTransformDirty();
}