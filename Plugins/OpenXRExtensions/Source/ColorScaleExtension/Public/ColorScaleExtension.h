// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <OpenXRCommon.h>
#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleManager.h"
#include "IIdentifiableXRDevice.h"


class COLORSCALEEXTENSION_API FColorScaleExtensionModule :
	public IOpenXRExtensionPlugin, 
	public IModuleInterface
{
public:
	FColorScaleExtensionModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void SetColorScaleAndBias(const FLinearColor& Scale, const FLinearColor& Bias);

	bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions);
	virtual const void* OnEndProjectionLayer(XrSession InSession, int32 InLayerIndex, const void* InNext, XrCompositionLayerFlags& OutFlags) override;

private:
	XrCompositionLayerColorScaleBiasKHR HMDLayerColorInfo;
};