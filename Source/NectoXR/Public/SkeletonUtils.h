// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Animation/Skeleton.h"
#include "SkeletonUtils.generated.h"

/**
 * 
 */
UCLASS()
class NECTOXR_API USkeletonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//#if WITH_EDITORONLY_DATA
//		UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skeleton")
//		static TArray<int32> GetChildBoneIndexes(USkeletalMesh* SkeletalMesh, int32 ParentBoneIndex);
//#endif

		UFUNCTION(BlueprintCallable, Category = "Physical Animation")
		static void ForceTeleport(UPhysicalAnimationComponent* PhysicalAnimationComponent);
};
