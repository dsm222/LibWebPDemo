// Fill out your copyright notice in the Description page of Project Settings.


#include "SampleWebPBPLibrary.h"

#include "decode.h"
#include "demux.h"
#include "encode.h"
#include "LogSampleWebP.h"
#include "mux.h"
#include "SampleWebPCore.h"
#include "SampleWebPMultiShotSubsystem.h"
#include "SampleWebpShowMultiSubsystem.h"
#include "Kismet/GameplayStatics.h"

bool USampleWebPBPLibrary::GetSampleWebpVersion(FString& Version)
{
	int32 DecoderVersion = WebPGetDecoderVersion();
	int32 DemuxVersion = WebPGetDemuxVersion();
	int32 MuxVersion = WebPGetMuxVersion();
	int32 EncoderVersion = WebPGetEncoderVersion();

	Version = FString::Printf(TEXT("DecoderVersion: %d, DemuxVersion: %d, MuxVersion: %d, EncoderVersion: %d"), DecoderVersion, DemuxVersion, MuxVersion, EncoderVersion);

	UE_LOG(LogSampleWebP, Log, TEXT("%s"), *Version);
	return true;
}

void USampleWebPBPLibrary::BeginRecord(UObject* WorldContextObject, FString InGeneratedWebpPicturesPath,
	FSampleWebpPictureInformation InWebpPictureInformation, bool& bBegin)
{
	bBegin = false;
	if (!WorldContextObject || !WorldContextObject->GetWorld())
		return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	USampleWebPMultiShotSubsystem* XGWebpSubSystem = GameInstance->GetSubsystem<USampleWebPMultiShotSubsystem>();
	TSharedPtr<FSampleWebpPictureInformation> XGWebpPictureInformationPtr = MakeShareable(new FSampleWebpPictureInformation(InWebpPictureInformation));
	
	bBegin = XGWebpSubSystem->BeginRecord(InGeneratedWebpPicturesPath, XGWebpPictureInformationPtr);
}

void USampleWebPBPLibrary::BeginRecordFullViewport(UObject* WorldContextObject, FString InGeneratedWebpPicturesPath,
	bool& bBegin)
{
	bBegin = false;
	if (!WorldContextObject || !WorldContextObject->GetWorld())
		return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	USampleWebPMultiShotSubsystem* WebpSubSystem = GameInstance->GetSubsystem<USampleWebPMultiShotSubsystem>();

	FVector2D ViewportSize;
	if (FSampleWebPCore::GetViewportSize(WorldContextObject, ViewportSize))
	{

		TSharedPtr<FSampleWebpPictureInformation> WebpPictureInformation = MakeShareable(new FSampleWebpPictureInformation(0, 0, ViewportSize.X - 1, ViewportSize.Y - 1));
		UE_LOG(LogTemp, Warning, TEXT("WebpPictureInformation: %d, %d, %d, %d"), WebpPictureInformation->X0, WebpPictureInformation->X1, WebpPictureInformation->Y1, WebpPictureInformation->Y0);
		bBegin = WebpSubSystem->BeginRecord(InGeneratedWebpPicturesPath, WebpPictureInformation);
	}
}

void USampleWebPBPLibrary::EndRecord(UObject* WorldContextObject, FWebpFinishGenerateMultiWebp InFinishWebpBPDegelete)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
		return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	USampleWebPMultiShotSubsystem* WebpSubSystem = GameInstance->GetSubsystem<USampleWebPMultiShotSubsystem>();

	WebpSubSystem->EndRecord(InFinishWebpBPDegelete);
}

void USampleWebPBPLibrary::LoadWebp(UObject* WorldContextObject, FWebpLoadAndShowWebp InLoadAndShowWebpDegelete,
	FString InWebpFilePath)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		InLoadAndShowWebpDegelete.ExecuteIfBound(false,nullptr,-1,-1);
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	USampleWebpShowMultiSubsystem* WebpShowSubSystem = GameInstance->GetSubsystem<USampleWebpShowMultiSubsystem>();

	WebpShowSubSystem->LoadWebp(InLoadAndShowWebpDegelete, InWebpFilePath);
}

void USampleWebPBPLibrary::ReleaseLoadedWebp(UObject* WorldContextObject)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld()) return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	USampleWebpShowMultiSubsystem* WebpShowSubSystem = GameInstance->GetSubsystem<USampleWebpShowMultiSubsystem>();

	WebpShowSubSystem->ReleaseLoadedWebp();
}
