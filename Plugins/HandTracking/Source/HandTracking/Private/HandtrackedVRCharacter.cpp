// Fill out your copyright notice in the Description page of Project Settings.

#include "HandtrackedVRCharacter.h"
#include "VRBaseCharacter.h"
#include "GripMotionControllerComponent.h"

void AHandtrackedVRCharacter::AddMotionController(UGripMotionControllerComponent* GripMotionController)
{
	if (GripMotionController) {
		GripMotionController->SetupAttachment(NetSmoother);
		GripMotionController->SetTrackingMotionSource(FXRMotionControllerBase::LeftHandSourceId);
		GripMotionController->bOffsetByHMD = false;
		GripMotionController->AddTickPrerequisiteComponent(GetCharacterMovement());
		GripMotionController->OverrideSendTransform = &AVRBaseCharacter::Server_SendTransformLeftController;

		if (GripMotionController->GetTrackingSource() == EControllerHand::Left) {
			LeftMotionController->DestroyComponent();
			LeftMotionController = GripMotionController;
		}
		else if (GripMotionController->GetTrackingSource() == EControllerHand::Right) {
			RightMotionController->DestroyComponent();
			RightMotionController = GripMotionController;
		}
	}
}
