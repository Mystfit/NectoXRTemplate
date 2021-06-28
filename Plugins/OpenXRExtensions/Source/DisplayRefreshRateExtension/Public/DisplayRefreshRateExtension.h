// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <OpenXRCommon.h>
#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleManager.h"

//Forwards
class FOpenXRHMD;


class DISPLAYREFRESHRATEEXTENSION_API FDisplayRefreshRateExtensionModule :
	public IOpenXRExtensionPlugin, 
	public IModuleInterface
{
public:
	FDisplayRefreshRateExtensionModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual const void* OnGetSystem(XrInstance InInstance, const void* InNext) override;
	bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions);
	FOpenXRHMD* GetOpenXRHMD() const;

	float GetRefreshRate();
	void SetRefreshRate(float RefreshRate);
	TArray<float> EnumerateRefreshRates();

private:
	// Extension functions
	PFN_xrGetDisplayRefreshRateFB xrGetDisplayRefreshRateFB = nullptr;
	PFN_xrRequestDisplayRefreshRateFB xrRequestDisplayRefreshRateFB = nullptr;
	PFN_xrEnumerateDisplayRefreshRatesFB xrEnumerateDisplayRefreshRatesFB = nullptr;

	bool bIsExtensionAvailable;
};
