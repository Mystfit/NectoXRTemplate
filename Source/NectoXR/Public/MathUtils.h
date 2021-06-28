// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathUtils.generated.h"

/**
 * 
 */
UCLASS()
class NECTOXR_API UMathUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Exec, Category = "Collision")
	//static void GetSceneComponentArrayBounds(const TArray<USceneComponent*>& Components, bool bOnlyCollidingComponents, FVector& Center, FVector& BoxExtent);
	static void GetSceneComponentArraySphereBounds(const TArray<USceneComponent*>& Components, bool bOnlyCollidingComponents, FVector& Center, FVector& BoxExtent, float& SphereExtentRadius);
};
