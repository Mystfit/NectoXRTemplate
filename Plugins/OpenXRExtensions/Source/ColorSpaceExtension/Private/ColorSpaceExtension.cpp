#include "ColorSpaceExtension.h"
#include "OpenXRCore.h"
#include <OpenXRHMD/Private/OpenXRHMD.h>

#define LOCTEXT_NAMESPACE "FColorSpaceExtensionModule"


FColorSpaceExtensionModule::FColorSpaceExtensionModule() : bIsExtensionAvailable(false)
{
}

void FColorSpaceExtensionModule::StartupModule()
{
	// Register extension with OpenXR
	RegisterOpenXRExtensionModularFeature();
}

void FColorSpaceExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

const void* FColorSpaceExtensionModule::OnGetSystem(XrInstance InInstance, const void* InNext)
{
	// Store extension open xr calls to member function pointers for convenient use.
	XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrEnumerateColorSpacesFB", (PFN_xrVoidFunction*)&xrEnumerateColorSpacesFB));
	XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrSetColorSpaceFB", (PFN_xrVoidFunction*)&xrSetColorSpaceFB));
	bIsExtensionAvailable = true;
	return InNext;
}

const void* FColorSpaceExtensionModule::OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext)
{
	XrSystemColorSpacePropertiesFB ColorSpaceSystemProperties {XR_TYPE_SYSTEM_COLOR_SPACE_PROPERTIES_FB };
	XrSystemProperties systemProperties{ XR_TYPE_SYSTEM_PROPERTIES, &ColorSpaceSystemProperties };

	XR_ENSURE(xrGetSystemProperties(InInstance, InSystem, &systemProperties));
	StartupColorSpace = ColorSpaceSystemProperties.colorSpace;

	return InNext;
}

bool FColorSpaceExtensionModule::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	OutExtensions.Add(XR_FB_COLOR_SPACE_EXTENSION_NAME);
	return true;
}

FOpenXRHMD* FColorSpaceExtensionModule::GetOpenXRHMD() const
{
	static FName SystemName(TEXT("OpenXR"));
	if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName))
	{
		return static_cast<FOpenXRHMD*>(GEngine->XRSystem.Get());
	}
	return nullptr;	
}

void FColorSpaceExtensionModule::SetColorSpace(XrColorSpaceFB ColorSpace)
{
	if (!bIsExtensionAvailable)
		return;

	if (XrSession Session = GetOpenXRHMD()->GetSession()) {
		XR_ENSURE(xrSetColorSpaceFB(Session, ColorSpace));
	}
}

TArray<XrColorSpaceFB> FColorSpaceExtensionModule::EnumerateColorSpaces()
{
	TArray<XrColorSpaceFB> nativeColorSpaces;

	if (!bIsExtensionAvailable || !GetOpenXRHMD())
		return nativeColorSpaces;

	if (XrSession Session = GetOpenXRHMD()->GetSession()) {

		// Get num color spaces available
		uint32_t colorSpaceCount = 0;
		XR_ENSURE(xrEnumerateColorSpacesFB(Session, 0, &colorSpaceCount, nullptr));
		nativeColorSpaces.SetNum(colorSpaceCount);

		// Get color spaces buffer
		XR_ENSURE(xrEnumerateColorSpacesFB(Session, colorSpaceCount, &colorSpaceCount, nativeColorSpaces.GetData()));
	}
	
	return nativeColorSpaces;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FColorSpaceExtensionModule, ColorSpaceExtension)