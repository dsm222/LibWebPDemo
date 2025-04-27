// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SampleWebPType.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FWebpFinishGenerateMultiWebp, bool, bFinishGenerate);

DECLARE_DYNAMIC_DELEGATE_FourParams(FWebpLoadAndShowWebp, bool, bLoad, UTexture2D*, OutWebpPicture, int32, WebpWidth, int32, WebpHeight);

USTRUCT(BlueprintType)
struct SAMPLEWEBP_API FSampleWebpPictureInformation
{
	GENERATED_USTRUCT_BODY()

public:
	FSampleWebpPictureInformation();
	FSampleWebpPictureInformation(int32 InX0, int32 InY0, int32 InX1, int32 InY1);
	FSampleWebpPictureInformation(FSampleWebpPictureInformation& InWebpPictureInformation);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webp")
	int32 X0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webp")
	int32 Y0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webp")
	int32 X1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Webp")
	int32 Y1;

	int32 GetPictureWidth();
	int32 GetPictureHeight();

	void ResetPictureInformation();
};

UENUM()
enum class ESampleWebpProcessType :uint8
{
	None,
	Recording,
	Generating,
	Max
};

UENUM()
enum class ESampleWebpLoadAndShowType :uint8
{
	None,
	Loading,
	Showing,
	Max
};
