// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "GripMotionControllerComponent.h"
#include "HandtrackedVRCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HANDTRACKING_API AHandtrackedVRCharacter : public AVRCharacter
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void AddMotionController(UGripMotionControllerComponent* GripMotionController);
};
