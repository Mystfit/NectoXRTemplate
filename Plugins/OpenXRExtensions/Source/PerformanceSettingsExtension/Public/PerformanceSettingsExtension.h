// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <OpenXRCommon.h>
#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleManager.h"
#include "IIdentifiableXRDevice.h"

//Forwards
class FOpenXRHMD;


class PERFORMANCESETTINGSEXTENSION_API FPerformanceSettingsExtensionModule :
	public IOpenXRExtensionPlugin, 
	public IModuleInterface
{
public:
	FPerformanceSettingsExtensionModule();

	virtual const void* OnGetSystem(XrInstance InInstance, const void* InNext) override;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions);
	FOpenXRHMD* GetOpenXRHMD() const;

	void SetCpuPerformanceHint(XrPerfSettingsLevelEXT CpuLevel);
	void SetGpuPerformanceHint(XrPerfSettingsLevelEXT GpuLevel);

private:
	// Extension functions
	PFN_xrPerfSettingsSetPerformanceLevelEXT  pfnPerfSettingsSetPerformanceLevelEXT = nullptr;

	bool bIsExtensionAvailable;
};