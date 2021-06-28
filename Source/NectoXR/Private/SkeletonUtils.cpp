// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonUtils.h"
#include "Animation/Skeleton.h"
#include "SkeletonUtils.h"

//TArray<int32> USkeletonUtils::GetChildBoneIndexes(USkeletalMesh* SkeletalMesh, int32 ParentBoneIndex)
//{
//	check(SkeletalMesh);
//
//	auto outbones = TArray<int32>();
//	SkeletalMesh->Skeleton->GetChildBones(ParentBoneIndex, outbones);
//	return outbones;
//}

void USkeletonUtils::ForceTeleport(UPhysicalAnimationComponent* PhysicalAnimationComponent)
{
	check(PhysicalAnimationComponent);
	//PhysicalAnimationComponent->OnTeleport();
}
