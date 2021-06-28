#include "OpenXRExtensionsFunctionLibrary.h"
#include "Modules/ModuleManager.h"

#if defined(OPENXR_SUPPORTED)
#include "OpenXRCommon.h"
#include "DisplayRefreshRateExtension.h"
#include "ColorScaleExtension.h"
#include "ColorSpaceExtension.h"
#include "PerformanceSettingsExtension.h"
#else
class FDisplayRefreshRateExtensionModule {};
class FColorSpaceExtensionModule {};
class FColorScaleExtensionModule {};
class FPerformanceSettingsExtensionModule {};
#endif

DEFINE_LOG_CATEGORY(LogOpenXRExtension);

UOpenXRExtensionsFunctionLibrary::UOpenXRExtensionsFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOpenXRExtensionsFunctionLibrary::SetColorScaleAndBias(FLinearColor ColorScale, FLinearColor ColorBias)
{
#if defined(OPENXR_SUPPORTED)
	if (auto module = UOpenXRExtensionsFunctionLibrary::GetColorScaleModule()) {
		module->SetColorScaleAndBias(ColorScale, ColorBias);
	}
#endif
}

void UOpenXRExtensionsFunctionLibrary::SetColorSpace(const FXrColorSpaceFB ColorSpace)
{
#if defined(OPENXR_SUPPORTED)
	if(auto module = UOpenXRExtensionsFunctionLibrary::GetColorSpaceModule()){
		module->SetColorSpace(static_cast<XrColorSpaceFB>(ColorSpace));
	}
#endif
}

FXrColorSpaceFB UOpenXRExtensionsFunctionLibrary::GetNativeColorSpace()
{
#if defined(OPENXR_SUPPORTED)
	if (auto module = UOpenXRExtensionsFunctionLibrary::GetColorSpaceModule()) {
		return static_cast<FXrColorSpaceFB>(module->StartupColorSpace);
	}
#endif
	return FXrColorSpaceFB::COLOR_SPACE_UNMANAGED;
}

TArray<FXrColorSpaceFB> UOpenXRExtensionsFunctionLibrary::EnumerateColorSpaces()
{
#if defined(OPENXR_SUPPORTED)
	if(auto module = UOpenXRExtensionsFunctionLibrary::GetColorSpaceModule()){
		TArray<FXrColorSpaceFB> colorSpaces;
		for (auto space : module->EnumerateColorSpaces()) {
			colorSpaces.Add((FXrColorSpaceFB)space);
		}
		return colorSpaces;
	}
#endif
	return TArray<FXrColorSpaceFB>();
}

void UOpenXRExtensionsFunctionLibrary::SetRefreshRate(float RefreshRate)
{
#if defined(OPENXR_SUPPORTED)
	if(auto module = UOpenXRExtensionsFunctionLibrary::GetRefreshRateModule()){
		module->SetRefreshRate(RefreshRate);
	}
#endif
}

float UOpenXRExtensionsFunctionLibrary::GetRefreshRate()
{
#if defined(OPENXR_SUPPORTED)
	if(auto module = UOpenXRExtensionsFunctionLibrary::GetRefreshRateModule()){
		return module->GetRefreshRate();
	}
#endif
	return 0;
}

TArray<float> UOpenXRExtensionsFunctionLibrary::EnumerateRefreshRates()
{
#if defined(OPENXR_SUPPORTED)
	if(auto module = UOpenXRExtensionsFunctionLibrary::GetRefreshRateModule()){
		return module->EnumerateRefreshRates();
	}
#endif
	return TArray<float>();
}

void UOpenXRExtensionsFunctionLibrary::SetCpuPerformanceHint(FXrPerfSettingsLevelEXT CpuLevel)
{
#if defined(OPENXR_SUPPORTED)
	if (auto module = UOpenXRExtensionsFunctionLibrary::GetPerformanceSettingsModule()) {
		module->SetCpuPerformanceHint((XrPerfSettingsLevelEXT)CpuLevel);
	}
#endif
}

void UOpenXRExtensionsFunctionLibrary::SetGpuPerformanceHint(FXrPerfSettingsLevelEXT GpuLevel)
{
#if defined(OPENXR_SUPPORTED)
	if (auto module = UOpenXRExtensionsFunctionLibrary::GetPerformanceSettingsModule()) {
		module->SetGpuPerformanceHint((XrPerfSettingsLevelEXT)GpuLevel);
	}
#endif
}

FDisplayRefreshRateExtensionModule* UOpenXRExtensionsFunctionLibrary::GetRefreshRateModule(){
#if defined(OPENXR_SUPPORTED)
	IModuleInterface* module = FModuleManager::Get().GetModule("DisplayRefreshRateExtension");
	if (module) {
		return static_cast<FDisplayRefreshRateExtensionModule*>(module);
	}
#endif
	UE_LOG(LogOpenXRExtension, Warning, TEXT("Could not find the DisplayRefreshRateExtension module"));
	return nullptr;
}

FColorSpaceExtensionModule* UOpenXRExtensionsFunctionLibrary::GetColorSpaceModule(){
#if defined(OPENXR_SUPPORTED)
	IModuleInterface* module = FModuleManager::Get().GetModule("ColorSpaceExtension");
	if (module) {
		return static_cast<FColorSpaceExtensionModule*>(module);
	}
#endif
	UE_LOG(LogOpenXRExtension, Warning, TEXT("Could not find the ColorSpaceExtension module"));
	return nullptr;
}

FColorScaleExtensionModule* UOpenXRExtensionsFunctionLibrary::GetColorScaleModule(){
#if defined(OPENXR_SUPPORTED)
	IModuleInterface* module = FModuleManager::Get().GetModule("ColorScaleExtension");
	if (module) {
		return static_cast<FColorScaleExtensionModule*>(module);
	}
#endif
	UE_LOG(LogOpenXRExtension, Warning, TEXT("Could not find the ColorScaleExtension module"));
	return nullptr;
}

FPerformanceSettingsExtensionModule* UOpenXRExtensionsFunctionLibrary::GetPerformanceSettingsModule()
{
#if defined(OPENXR_SUPPORTED)
	IModuleInterface* module = FModuleManager::Get().GetModule("PerformanceSettingsExtension");
	if (module) {
		return static_cast<FPerformanceSettingsExtensionModule*>(module);
	}
#endif
	UE_LOG(LogOpenXRExtension, Warning, TEXT("Could not find the PerformanceSettingsExtension module"));
	return nullptr;
}
