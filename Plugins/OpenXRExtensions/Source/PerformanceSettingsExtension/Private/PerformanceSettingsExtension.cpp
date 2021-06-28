// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "PerformanceSettingsExtension.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "OpenXRCore.h"
#include <OpenXRHMD/Private/OpenXRHMD.h>

#define LOCTEXT_NAMESPACE "FPerformanceSettingsExtensionModule"


FPerformanceSettingsExtensionModule::FPerformanceSettingsExtensionModule() : 
	bIsExtensionAvailable(false)
{
}

void FPerformanceSettingsExtensionModule::StartupModule()
{
	// Register extension with OpenXR
	RegisterOpenXRExtensionModularFeature();
}

void FPerformanceSettingsExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

const void* FPerformanceSettingsExtensionModule::OnGetSystem(XrInstance InInstance, const void* InNext)
{
	// Store extension open xr calls to member function pointers for convenient use.
	XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrPerfSettingsSetPerformanceLevelEXT", (PFN_xrVoidFunction*)&pfnPerfSettingsSetPerformanceLevelEXT));
	bIsExtensionAvailable = true;
	return InNext;
}


bool FPerformanceSettingsExtensionModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	OutExtensions.Add(XR_EXT_PERFORMANCE_SETTINGS_EXTENSION_NAME);
	return true;
}


FOpenXRHMD* FPerformanceSettingsExtensionModule::GetOpenXRHMD() const
{
	static FName SystemName(TEXT("OpenXR"));
	if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName))
	{
		return static_cast<FOpenXRHMD*>(GEngine->XRSystem.Get());
	}
	return nullptr;
}


void FPerformanceSettingsExtensionModule::SetCpuPerformanceHint(XrPerfSettingsLevelEXT CpuLevel)
{
	if (!bIsExtensionAvailable)
		return;

	if (XrSession Session = GetOpenXRHMD()->GetSession()) {
		XR_ENSURE(pfnPerfSettingsSetPerformanceLevelEXT(Session, XR_PERF_SETTINGS_DOMAIN_CPU_EXT, CpuLevel));
	}
}

void FPerformanceSettingsExtensionModule::SetGpuPerformanceHint(XrPerfSettingsLevelEXT GpuLevel)
{
	if (!bIsExtensionAvailable)
		return;

	if (XrSession Session = GetOpenXRHMD()->GetSession()) {
		XR_ENSURE(pfnPerfSettingsSetPerformanceLevelEXT(Session, XR_PERF_SETTINGS_DOMAIN_GPU_EXT, GpuLevel));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPerformanceSettingsExtensionModule, PerformanceSettingsExtension)