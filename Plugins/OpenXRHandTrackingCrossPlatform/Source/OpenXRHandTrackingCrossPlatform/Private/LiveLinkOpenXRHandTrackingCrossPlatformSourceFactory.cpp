// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiveLinkOpenXRHandTrackingCrossPlatformSourceFactory.h"
#include "IOpenXRHandTrackingCrossPlatformModule.h"
#include "OpenXRHandTrackingCrossPlatform.h"

#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"

#define LOCTEXT_NAMESPACE "OpenXRHandTrackingCrossPlatform"

FText ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("HandTrackingLiveLinkSourceName", "OpenXR Hand Tracking Source");
}

FText ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("HandTrackingLiveLinkSourceTooltip", "OpenXR Hand Tracking Key Points Source");
}

ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory::EMenuType ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory::GetMenuType() const
{
	if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
	{
		ILiveLinkClient& LiveLinkClient = IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);

		if (!IOpenXRHandTrackingCrossPlatformModule::Get().IsLiveLinkSourceValid() || !LiveLinkClient.HasSourceBeenAdded(IOpenXRHandTrackingCrossPlatformModule::Get().GetLiveLinkSource()))
		{
			return EMenuType::MenuEntry;
		}
	}
	return EMenuType::Disabled;
}

TSharedPtr<ILiveLinkSource> ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory::CreateSource(const FString& ConnectionString) const
{
	return IOpenXRHandTrackingCrossPlatformModule::Get().GetLiveLinkSource();
}

#undef LOCTEXT_NAMESPACE
