#include "SampleWebPMultiShotSubsystem.h"

#include "SampleWebPCore.h"

bool USampleWebPMultiShotSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void USampleWebPMultiShotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SampleWebPMultiCallBack.BindUFunction(this, "SampleWebPOneMultiCallBackMethod");
	WebpPictureInformation = MakeShareable(new FSampleWebpPictureInformation);
}

void USampleWebPMultiShotSubsystem::Deinitialize()
{
	SampleWebPMultiCallBack.Unbind();
	ResetRecord();
	Super::Deinitialize();
}

void USampleWebPMultiShotSubsystem::Tick(float DeltaTime)
{
	if (ProcessType == ESampleWebpProcessType::Recording)
	{
		FScreenshotRequest::RequestScreenshot(false);
		if (!WebpTimestepMillisecond.Num())
			WebpTimestepMillisecond.Add(0);
		else
			WebpTimestepMillisecond.Add(DeltaTime * 1000);
	}
}

bool USampleWebPMultiShotSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId USampleWebPMultiShotSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGSampleWebPMultiShotSubsystem, STATGROUP_Tickables);
}

bool USampleWebPMultiShotSubsystem::BeginRecord(FString& InGeneratedWebpPicturesPath,
	TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation)
{
	if (!GetWorld())
		return false;
	if (ProcessType != ESampleWebpProcessType::None)
		return false;
	if (!FSampleWebPCore::CheckInWebpPictureSize(GetWorld(), InWebpPictureInformation))
		return false;

	ProcessType = ESampleWebpProcessType::Recording;
	GeneratedWebpPicturesPath = InGeneratedWebpPicturesPath;
	WebpPictureInformation = InWebpPictureInformation;
	
	FlushRenderingCommands();
	ScreenHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this,
		&USampleWebPMultiShotSubsystem::ScreenShotCallback);
	
	return true;
}

void USampleWebPMultiShotSubsystem::EndRecord(FWebpFinishGenerateMultiWebp& InFinshWebpBPDelegate)
{
	if (ProcessType != ESampleWebpProcessType::Recording)
	{
		InFinshWebpBPDelegate.ExecuteIfBound(false);
		return;
	}
	
	ProcessType = ESampleWebpProcessType::Generating;
	FinshWebpBPDelegate = InFinshWebpBPDelegate;

	AsyncTask(ENamedThreads::AnyThread, [this]()
	{
		//FPlatformProcess::Sleep(0.2);
		FScopeLock Lock(&WebpMutex);
		bool GenerateWebp = FSampleWebPCore::GenerateDynamicWebpPicture(
				GeneratedWebpPicturesPath,
				WebpPictureInformation,
				WebPColor,
				WebpTimestepMillisecond
			);

		AsyncTask(ENamedThreads::GameThread, [this, GenerateWebp]() {
				SampleWebPMultiCallBack.ExecuteIfBound(GenerateWebp);
				});
	});
}

void USampleWebPMultiShotSubsystem::ResetRecord()
{
	ProcessType = ESampleWebpProcessType::None;
	if (ScreenHandle.IsValid())
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);

	GeneratedWebpPicturesPath = TEXT("");
	WebpPictureInformation->ResetPictureInformation();
	WebPColor.Empty();
	WebpTimestepMillisecond.Empty();
}

void USampleWebPMultiShotSubsystem::ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{
	AsyncTask(ENamedThreads::AnyThread, [&, InWidth, InHeight, InColors]()
	{
		FScopeLock Lock(&WebpMutex);
		TArray<FColor> OutColors;

		for (int32 IndexY = WebpPictureInformation->Y0; IndexY <= WebpPictureInformation->Y1; ++IndexY)
		{
			for (int32 IndexX = WebpPictureInformation->X0; IndexX <= WebpPictureInformation->X1; ++IndexX)
			{
				int32 IndexArray = IndexY * InWidth + IndexX;
				OutColors.Add(InColors[IndexArray]);
			}
		}

		if (OutColors.Num() == (WebpPictureInformation->X1 - WebpPictureInformation->X0 + 1) * (WebpPictureInformation->Y1 - WebpPictureInformation->Y0 + 1))
			WebPColor.Add(MoveTemp(OutColors));
	});
}

void USampleWebPMultiShotSubsystem::SampleWebPOneMultiCallBackMethod(bool bGenerateWebp)
{
	if (ScreenHandle.IsValid())
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	ResetRecord();
	FinshWebpBPDelegate.ExecuteIfBound(bGenerateWebp);
}
