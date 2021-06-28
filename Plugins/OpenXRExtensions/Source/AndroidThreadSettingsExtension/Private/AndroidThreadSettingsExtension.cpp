// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AndroidThreadSettingsExtension.h"
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "HAL/ThreadManager.h"
#include "HAL/RunnableThread.h"
#include "OpenXRCore.h"
#include <OpenXRHMD/Private/OpenXRHMD.h>

#define LOCTEXT_NAMESPACE "FAndroidThreadSettingsExtensionModule"

#if PLATFORM_ANDROID
static TMap<EThreadPriority, XrAndroidThreadTypeKHR> ThreadPriorityToXRAppThreadPriority = {
	 {EThreadPriority::TPri_Normal, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR},
	 {EThreadPriority::TPri_AboveNormal, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR},
	 {EThreadPriority::TPri_BelowNormal, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR},
	 {EThreadPriority::TPri_Highest, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_WORKER_KHR},
	 {EThreadPriority::TPri_Lowest, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR},
	 {EThreadPriority::TPri_SlightlyBelowNormal, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR},
	 {EThreadPriority::TPri_TimeCritical, XrAndroidThreadTypeKHR::XR_ANDROID_THREAD_TYPE_APPLICATION_WORKER_KHR}
};
#endif

FAndroidThreadSettingsExtensionModule::FAndroidThreadSettingsExtensionModule() : 
	bIsExtensionAvailable(false)
{
}

void FAndroidThreadSettingsExtensionModule::StartupModule()
{
	// Register extension with OpenXR
	RegisterOpenXRExtensionModularFeature();
}

void FAndroidThreadSettingsExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

const void* FAndroidThreadSettingsExtensionModule::OnGetSystem(XrInstance InInstance, const void* InNext)
{
#if PLATFORM_ANDROID
	// Store extension open xr calls to member function pointers for convenient use.
	XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrSetAndroidApplicationThreadKHR", (PFN_xrVoidFunction*)&pfnAndroidThreadSetAndroidApplicationThreadKHR));
	bIsExtensionAvailable = true;
#endif
	return InNext;
}


bool FAndroidThreadSettingsExtensionModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
#if PLATFORM_ANDROID
	OutExtensions.Add(XR_KHR_ANDROID_THREAD_SETTINGS_EXTENSION_NAME);
#endif
	return true;
}


FOpenXRHMD* FAndroidThreadSettingsExtensionModule::GetOpenXRHMD() const
{
	static FName SystemName(TEXT("OpenXR"));
	if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName))
	{
		return static_cast<FOpenXRHMD*>(GEngine->XRSystem.Get());
	}
	return nullptr;
}

void FAndroidThreadSettingsExtensionModule::SetAllXRThreadPriorities() {
#if PLATFORM_ANDROID
	FThreadManager::Get().ForEachThread([this](uint32 ThreadID, FRunnableThread* thread) {
		XrAndroidThreadTypeKHR Priority;

		UE_LOG(LogTemp, Log, TEXT("Thread name: %s Priority: %d ID: %d"), *thread->GetThreadName(), thread->GetThreadPriority(), ThreadID);

		FString ThreadName = FThreadManager::GetThreadName(ThreadID);
		if (ThreadName == "RenderThread") {
			// Categorize the render thread as important
			Priority = XR_ANDROID_THREAD_TYPE_RENDERER_MAIN_KHR;
		} else if (ThreadName == "RHIThread") {
			// Setting the RHI thread to anything throws an XR_ENSURE exception
			return;
		} else if (ThreadName == "GameThread") {
			// Explicitly setting the game thread priority
			Priority = XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR;
		}
		else if (ThreadName == "AudioMixerRenderThread(1)") {
			// Setting the AudioMixerRenderThread thread to XR_ANDROID_THREAD_TYPE_APPLICATION_WORKER_KHR throws an XR_ENSURE exception
			Priority = XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR;
		}
		else {
			Priority = (thread->GetThreadPriority() < ThreadPriorityToXRAppThreadPriority.Num()) ? ThreadPriorityToXRAppThreadPriority[thread->GetThreadPriority()] : XR_ANDROID_THREAD_TYPE_APPLICATION_MAIN_KHR;
		}

		SetAndroidThreadPriority(ThreadID, Priority);
	});
#endif
}

#if PLATFORM_ANDROID
void FAndroidThreadSettingsExtensionModule::SetAndroidThreadPriority(uint32_t ThreadID, XrAndroidThreadTypeKHR Priority)
{
	if (!bIsExtensionAvailable)
 		return;

 	if (XrSession Session = GetOpenXRHMD()->GetSession()) {
		UE_LOG(LogTemp, Log, TEXT("Setting thread ID: %d to Priority: %d"), ThreadID, Priority);
 		XR_ENSURE(pfnAndroidThreadSetAndroidApplicationThreadKHR(Session, Priority, ThreadID));
 	}
}
#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAndroidThreadSettingsExtensionModule, AndroidThreadSettingsExtension)