#include "OpenXRExtensions.h"
#include "HAL/ThreadManager.h"
#include "HAL/RunnableThread.h"
#include "UObject/Class.h"

#if PLATFORM_ANDROID
#include "AndroidThreadSettingsExtension.h"
#endif

#define LOCTEXT_NAMESPACE "FOpenXRExtensionsModule"


FOpenXRExtensionsModule::FOpenXRExtensionsModule()
{
}

void FOpenXRExtensionsModule::StartupModule()
{
#if PLATFORM_ANDROID
	//Our module needs to load after the engine has loaded before setting XR thread priorities
	//The AndroidThreadSettings module needs to load earlier to register with the other OpenXR modules
	IModuleInterface* module = FModuleManager::Get().GetModule("AndroidThreadSettingsExtension");
	if (module) {
		FAndroidThreadSettingsExtensionModule* android_thread_module = static_cast<FAndroidThreadSettingsExtensionModule*>(module);
		android_thread_module->SetAllXRThreadPriorities();
	}
#endif
}

void FOpenXRExtensionsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOpenXRExtensionsModule, OpenXRExtensions)
