// Fill out your copyright notice in the Description page of Project Settings.


#include "HandInteractionFunctions.h"
//#include "OpenXRHandTracking.h"
//#include "IOpenXRHandTrackingModule.h"

FTransform UHandInteractionFunctions::TransformFromString(const FString& InStr)
{
	FTransform t;
	t.InitFromString(InStr);
	return t;
}

//void UHandInteractionFunctions::ShowtVirtualKeyboard(bool bShow, int32 UserIndex, TSharedPtr<IVirtualKeyboardEntry> TextEntryWidget) const
//{
//	FSlateApplication::ShowVirtualKeyboard(bShow, UserIndex, TextEntryWidget);
//}

//bool UHandInteractionFunctions::IsHandTrackingAvailable() const
//{
//	auto HandTracker = StaticCastSharedPtr<FOpenXRHandTracking, IInputDevice>(IOpenXRHandTrackingModule::Get().GetInputDevice());
//	if (HandTracker)
//		return HandTracker->IsHandTrackingSupportedByDevice();
//	return false;
//}
