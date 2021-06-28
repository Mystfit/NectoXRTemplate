#pragma once

#if PLATFORM_ANDROID
#define XR_USE_PLATFORM_ANDROID 1
#include <android_native_app_glue.h>
#endif

#include <openxr.h>
#include <openxr_platform.h>
