// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "MotionControllerComponent.h"
#include "InputCoreTypes.h"
#include "HeadMountedDisplayTypes.h"
#include "XRPoseableHandComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class HANDTRACKING_API UXRPoseableHandComponent : public UPoseableMeshComponent
{
	GENERATED_BODY()

public:
	UXRPoseableHandComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionControllerComponent* MotionControllerSource;

	UFUNCTION(BlueprintCallable)
	EControllerHand TrackedControllerType() const;

	UFUNCTION(BlueprintCallable)
	TMap<EHandKeypoint, FTransform> GetAllBoneTransforms() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EHandKeypoint, FName> BoneNameMapping;

private:
	/** Whether or not this component has authority within the frame */
	bool bHasAuthority;

	void UpdateBonePose();
};
