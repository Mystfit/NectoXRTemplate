// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ColorScaleExtension.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "OpenXRCore.h"

#define LOCTEXT_NAMESPACE "FColorScaleExtensionModule"


FColorScaleExtensionModule::FColorScaleExtensionModule() : 
	HMDLayerColorInfo {
		XR_TYPE_COMPOSITION_LAYER_COLOR_SCALE_BIAS_KHR,
		nullptr,
		XrColor4f{1.0f, 1.0f, 1.0f, 1.0f },
		XrColor4f{0.0f, 0.0f, 0.0f, 0.0f }
	}
{
}

void FColorScaleExtensionModule::StartupModule()
{
	// Register extension with OpenXR
	RegisterOpenXRExtensionModularFeature();
}

void FColorScaleExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FColorScaleExtensionModule::SetColorScaleAndBias(const FLinearColor& Scale, const FLinearColor& Bias)
{
	HMDLayerColorInfo.colorScale = XrColor4f{ Scale.R, Scale.G, Scale.B, Scale.A };
	HMDLayerColorInfo.colorBias = XrColor4f{ Bias.R, Bias.G, Bias.B, Bias.A };
}

bool FColorScaleExtensionModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	OutExtensions.Add(XR_KHR_COMPOSITION_LAYER_COLOR_SCALE_BIAS_EXTENSION_NAME);
	return true;
}

const void* FColorScaleExtensionModule::OnEndProjectionLayer(XrSession InSession, int32 InLayerIndex, const void* InNext, XrCompositionLayerFlags& OutFlags)
{
	HMDLayerColorInfo.next = InNext;
	return &HMDLayerColorInfo;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FColorScaleExtensionModule, ColorScaleExtension)