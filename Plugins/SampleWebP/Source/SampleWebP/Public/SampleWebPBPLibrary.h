// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "SampleWebPType.h"
#include "SampleWebPBPLibrary.generated.h"

struct FSampleWebpPictureInformation;
/**
 * 
 */
UCLASS()
class SAMPLEWEBP_API USampleWebPBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="SampleWebP")
	static bool GetSampleWebpVersion(FString& Version);

	UFUNCTION(BlueprintCallable, Category = "SampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "BeginRecord"))
	static void BeginRecord(
		UObject* WorldContextObject,
		FString InGeneratedWebpPicturesPath,
		FSampleWebpPictureInformation InWebpPictureInformation,
		bool& bBegin);

	UFUNCTION(BlueprintCallable, Category = "SampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "BeginRecordFullViewport"))
	static void BeginRecordFullViewport(
		UObject* WorldContextObject,
		FString InGeneratedWebpPicturesPath,
		bool& bBegin);

	UFUNCTION(BlueprintCallable, Category = "SampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "EndRecord"))
	static void EndRecord(UObject* WorldContextObject, FWebpFinishGenerateMultiWebp InFinishWebpBPDegelete);

	UFUNCTION(BlueprintCallable, Category = "SampleWebp", meta = (WorldContext = "WorldContextObject", DisplayName = "LoadWebp"))
	static	void LoadWebp(UObject* WorldContextObject, FWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath);
	UFUNCTION(BlueprintCallable, Category = "Webp", meta = (WorldContext = "WorldContextObject", DisplayName = "ReleaseLoadedWebp"))
	static void ReleaseLoadedWebp(UObject* WorldContextObject);
};
