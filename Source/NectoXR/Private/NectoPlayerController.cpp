// Fill out your copyright notice in the Description page of Project Settings.


#include "NectoPlayerController.h"

void ANectoPlayerController::BP_ChangeState_Spectator()
{
    ChangeState(NAME_Spectating);
    if ( this->GetLocalRole() == ROLE_Authority && PlayerState != NULL)
    {
        PlayerState->SetIsSpectator(true);
    }
}

void ANectoPlayerController::BP_ChangeState_Player()
{
    ChangeState(NAME_Playing);
    if (this->GetLocalRole() == ROLE_Authority && PlayerState != NULL)
    {
        PlayerState->SetIsSpectator(false);
    }
}