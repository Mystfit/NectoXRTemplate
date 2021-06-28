// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LiveLinkSourceFactory.h"
#include "LiveLinkOpenXRHandTrackingCrossPlatformSourceFactory.generated.h"

UCLASS()
class ULiveLinkOpenXRHandTrackingCrossPlatformSourceFactory : public ULiveLinkSourceFactory
{
public:

	GENERATED_BODY()

	virtual FText GetSourceDisplayName() const;
	virtual FText GetSourceTooltip() const;

	virtual EMenuType GetMenuType() const override;
	virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

	TSharedPtr<class SLiveLinkOpenXRHandTrackingCrossPlatformSourceEditor> ActiveSourceEditor;
};