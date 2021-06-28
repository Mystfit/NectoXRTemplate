// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OpenXRExtensionsFunctionLibrary.generated.h"

//Forwards
class FDisplayRefreshRateExtensionModule;
class FColorScaleExtensionModule;
class FColorSpaceExtensionModule;
class FPerformanceSettingsExtensionModule;

DECLARE_LOG_CATEGORY_EXTERN(LogOpenXRExtension, Log, All);


UENUM(BlueprintType)
enum class FXrColorSpaceFB :uint8 {
	COLOR_SPACE_UNMANAGED = 0  UMETA(DisplayName = "Unmanaged color"),
	COLOR_SPACE_REC2020 = 1  UMETA(DisplayName = "REC 2020 D65 white point"),
	COLOR_SPACE_REC709 = 2  UMETA(DisplayName = "REC709 (similar to sRGB)"),
	COLOR_SPACE_RIFT_CV1 = 3  UMETA(DisplayName = "Rift CV1"),
	COLOR_SPACE_RIFT_S = 4  UMETA(DisplayName = "Rift S"),
	COLOR_SPACE_QUEST = 5  UMETA(DisplayName = "Quest"),
	COLOR_SPACE_P3 = 6  UMETA(DisplayName = "DCI-P3 D65 white point"),
	COLOR_SPACE_ADOBE_RGB = 7  UMETA(DisplayName = "Adobe")
};

UENUM(BlueprintType)
enum class FXrPerfSettingsLevelEXT :uint8 {
	XR_PERF_SETTINGS_LEVEL_POWER_SAVINGS_EXT = 0      UMETA(DisplayName = "Power savings"),
	XR_PERF_SETTINGS_LEVEL_SUSTAINED_LOW_EXT = 25     UMETA(DisplayName = "Sustained low"),
	XR_PERF_SETTINGS_LEVEL_SUSTAINED_HIGH_EXT = 50    UMETA(DisplayName = "Sustained high"),
	XR_PERF_SETTINGS_LEVEL_BOOST_EXT = 75             UMETA(DisplayName = "Boost")
};


UCLASS()
class OPENXREXTENSIONS_API UOpenXRExtensionsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	// Color scale functions
	//----------------------

	/**
	 * Multiply the post-compositor frame against a color and add a bias.
	 * LayerColor = LayerColor * ColorScale + ColorBias
	 *
	 * @param ColorScale		(in) Color to multiply the compositor layer by
	 * @param ColorBias			(in) Color to offset the compositor layer by
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static void SetColorScaleAndBias(FLinearColor ColorScale, FLinearColor ColorBias);


	// Color space functions
	// ---------------------

	/**
	 * Set the HMD color space.
	 *
	 * @param ColorSpace		Color space to switch to
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static void SetColorSpace(const FXrColorSpaceFB ColorSpace);

	/**
	 * Get the native color space of the HMD.
	 *
	 * @param RefreshRate		Refresh rate in Hz
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static FXrColorSpaceFB GetNativeColorSpace();

	/**
	 * List all supported color spaces for the HMD
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static TArray<FXrColorSpaceFB> EnumerateColorSpaces();


	// Display refresh rate functions

	/**
	 * Set the HMD refresh rate.
	 *
	 * @param RefreshRate		Refresh rate in Hz
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static void SetRefreshRate(float RefreshRate);

	/**
	 * Get the current HMD refresh rate.
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static float GetRefreshRate();

	/**
	 * List all supported refresh rates for the HMD
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static TArray<float> EnumerateRefreshRates();


	// Performance setting functions

	/**
	 * Set the CPU performance level.
	 *
	 * @param CPUlevel		CPU performance level
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static void SetCpuPerformanceHint(FXrPerfSettingsLevelEXT CpuLevel);

	/**
	 * Set the GPU performance level.
	 *
	 * @param GPUlevel		GPU performance level
	 */
	UFUNCTION(BlueprintCallable, Category = "XR|HeadMountedDisplay")
	static void SetGpuPerformanceHint(FXrPerfSettingsLevelEXT GpuLevel);


private:
	static FDisplayRefreshRateExtensionModule* GetRefreshRateModule();
	static FColorSpaceExtensionModule* GetColorSpaceModule();
	static FColorScaleExtensionModule* GetColorScaleModule();
	static FPerformanceSettingsExtensionModule* GetPerformanceSettingsModule();
};
