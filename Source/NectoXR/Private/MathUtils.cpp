// Fill out your copyright notice in the Description page of Project Settings.

#include "MathUtils.h"
#include "Components/SceneComponent.h"

void UMathUtils::GetSceneComponentArraySphereBounds(const TArray<USceneComponent*>& Components, bool bOnlyCollidingComponents, FVector& Center, FVector& BoxExtent, float& SphereExtentRadius)
{
	FBoxSphereBounds ComponentBounds(ForceInit);
	// Iterate over actors and accumulate bouding box
	for (int32 ActorIdx = 0; ActorIdx < Components.Num(); ActorIdx++)
	{
		USceneComponent* C = Components[ActorIdx];
		// Check actor is non-null, not deleted
		if (C && !C->IsPendingKill())
		{
			ComponentBounds = ComponentBounds + C->Bounds;
		}
	}

	// if a valid box, get its center and extent
	Center = BoxExtent = FVector::ZeroVector;
	if (ComponentBounds.GetBox().IsValid)
	{
		Center = ComponentBounds.GetSphere().Center;
		BoxExtent = ComponentBounds.GetBox().GetExtent();
		SphereExtentRadius = ComponentBounds.GetSphere().W;
	}
}

//void UMathUtils::GetSceneComponentArraySphereBounds(const TArray<USceneComponent*>& Components, bool bOnlyCollidingComponents, FSphere& BoxSphereBounds)
//{
//	FSphere ComponentBounds(ForceInit);
//	// Iterate over actors and accumulate bouding box
//	for (int32 ComponentIdx = 0; ComponentIdx < Components.Num(); ComponentIdx++)
//	{
//		USceneComponent* C = Components[ComponentIdx];
//		// Check actor is non-null, not deleted
//		if (C && !C->IsPendingKill())
//		{
//			ComponentBounds += C->Bounds.GetSphere();
//		}
//	}
//
//	// if a valid box, get its center and extent
//	//Center = ComponentBounds.BoxExtent = FVector::ZeroVector;
//	if (ComponentBounds.IsValid)
//	{
//		BoxSphereBounds = ComponentBounds;
//	}
//}
