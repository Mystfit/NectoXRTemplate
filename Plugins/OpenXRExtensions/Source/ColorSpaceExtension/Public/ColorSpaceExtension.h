// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <OpenXRCommon.h>
#include "IOpenXRExtensionPlugin.h"
#include "Modules/ModuleManager.h"

//Forwards
class FOpenXRHMD;



class COLORSPACEEXTENSION_API FColorSpaceExtensionModule : 
	public IOpenXRExtensionPlugin, 
	public IModuleInterface
{
public:
	FColorSpaceExtensionModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual const void* OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext);
	virtual const void* OnGetSystem(XrInstance InInstance, const void* InNext) override;
	bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions);
	FOpenXRHMD* GetOpenXRHMD() const;

	void SetColorSpace(const XrColorSpaceFB ColorSpace);
	TArray<XrColorSpaceFB> EnumerateColorSpaces();

	XrColorSpaceFB StartupColorSpace;

private:
	// Extension functions
	PFN_xrEnumerateColorSpacesFB xrEnumerateColorSpacesFB = nullptr;
	PFN_xrSetColorSpaceFB xrSetColorSpaceFB = nullptr;

	bool bIsExtensionAvailable;
};
