// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"

struct FSampleWebPLib
{
	friend class FSampleWebPCore;

protected:
	static bool GenerateWebpByRGBA(const char* InWebpSavePath, const unsigned char* InRGBAData, int InWidth, int InHeight, int InQuality = 100);

	static bool GenerateDymaicWebpByRGBA(
		const char* InWebpSavePath,
		std::vector<const unsigned char*>& InRGBADatas,
		std::vector<int>                   InTimestamps_ms,
		int                                InWidth,
		int                                InHeight,
		float                              InQualityFactor = 100);

	static bool LoadDynamicWebpPictureByRGBA(
		const char* InWebpFilePath,
		std::vector<const unsigned char*>& OutRGBADatas,
		std::vector<int>& OutTimestamps_ms,
		int& OutWidth,
		int& OutHeight);
};