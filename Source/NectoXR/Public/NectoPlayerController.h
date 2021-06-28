// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRPlayerController.h"
#include "NectoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NECTOXR_API ANectoPlayerController : public AVRPlayerController
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = State)
        void BP_ChangeState_Spectator();

    UFUNCTION(BlueprintCallable, Category = State)
        void BP_ChangeState_Player();
};
