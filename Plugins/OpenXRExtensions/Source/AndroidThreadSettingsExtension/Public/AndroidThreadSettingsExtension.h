#pragma once

#include <OpenXRCommon.h>
#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleManager.h"
#include "IIdentifiableXRDevice.h"

//Forwards
class FOpenXRHMD;


class ANDROIDTHREADSETTINGSEXTENSION_API FAndroidThreadSettingsExtensionModule :
	public IOpenXRExtensionPlugin, 
	public IModuleInterface
{
public:
	FAndroidThreadSettingsExtensionModule();

	virtual const void* OnGetSystem(XrInstance InInstance, const void* InNext) override;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions);
	FOpenXRHMD* GetOpenXRHMD() const;

	void SetAllXRThreadPriorities();

#if PLATFORM_ANDROID
	void SetAndroidThreadPriority(uint32_t ThreadID, XrAndroidThreadTypeKHR Priority);

private:
	// Extension functions
	PFN_xrSetAndroidApplicationThreadKHR pfnAndroidThreadSetAndroidApplicationThreadKHR = nullptr;
#endif
	bool bIsExtensionAvailable;
};