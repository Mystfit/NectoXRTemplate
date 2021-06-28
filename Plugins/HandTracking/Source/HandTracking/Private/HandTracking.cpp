// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "HandTracking.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"

#define LOCTEXT_NAMESPACE "FHandTrackingModule"

void FHandTrackingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
}

void FHandTrackingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UnregisterSettings();
}

void FHandTrackingModule::RegisterSettings()
{
}

void FHandTrackingModule::UnregisterSettings()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHandTrackingModule, HandTracking)