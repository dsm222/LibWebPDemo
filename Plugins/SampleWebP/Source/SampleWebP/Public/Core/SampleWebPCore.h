// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FSampleWebpPictureInformation;

class SAMPLEWEBP_API FSampleWebPCore
{
public:
	static bool GenerateSaticWebpPicture(FString InPicturePath, const TArray<FColor>& InPictureColors, const FVector2D& InPictureSize, int32 InQuality = 100);

	static bool CheckWebpPicturePath(FString& InPicturePath);

	static bool GenerateDynamicWebpPicture(
		FString& InPicturePath,
		TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation,
		TArray<TArray<FColor>>& InPicturesColors,
		TArray<int32>& WebpTimestepMillisecond,
		int32                   InQualityFactor = 100);

	static bool GetViewportSize(UObject* WorldContextObject, FVector2D& OutViewportSize);

	static	bool CheckInWebpPictureSize(UObject* WorldContextObject, TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation);

	static bool LoadDynamicWebpPicture
	(
		FString InWebpFilePath,
		TArray<int32>& OutWebpTimestepMillisecond,
		TArray<TArray<FColor>>& OutPicturesColors,
		int32& OutWebpWidth,
		int32& OutWebpHeight
	);

};
