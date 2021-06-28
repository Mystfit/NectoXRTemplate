// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HandInteractionFunctions.generated.h"

/**
 * 
 */
UCLASS()
class HANDTRACKING_API UHandInteractionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	static FTransform TransformFromString(const FString& InStr);

	//UFUNCTION(BlueprintCallable)
	//bool IsHandTrackingAvailable() const;


	//UFUNCTION(BlueprintCallable, Exec)
	//void ShowtVirtualKeyboard(bool bShow, int32 UserIndex, TSharedPtr<IVirtualKeyboardEntry> TextEntryWidget = nullptr) const;
};
