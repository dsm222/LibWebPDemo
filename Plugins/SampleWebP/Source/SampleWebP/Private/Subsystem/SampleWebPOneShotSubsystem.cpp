#include "SampleWebPOneShotSubsystem.h"

#include "SampleWebPCore.h"

void USampleWebPOneShotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SampleWebPOneShotCallBack.BindUFunction(this, "SampleWebPOneShotCallBackMethod");
}

void USampleWebPOneShotSubsystem::Deinitialize()
{
	SampleWebPOneShotCallBack.Unbind();
	Super::Deinitialize();
}

bool USampleWebPOneShotSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void USampleWebPOneShotSubsystem::BeginSampleWebPOneShot(FSampleWebPOneShotCallBackBP InSampleWebPOneShotCallBackBP)
{
	if (bWorking)
	{
		InSampleWebPOneShotCallBackBP.ExecuteIfBound(false);
		return ;
	}
	bWorking =true;
	SampleWebPOneShotCallBackBP = InSampleWebPOneShotCallBackBP;

	FlushRenderingCommands();
	ScreenHandle = UGameViewportClient::OnScreenshotCaptured().AddUObject(this,
		&USampleWebPOneShotSubsystem::ScreenShotCallback);
	
	FScreenshotRequest::RequestScreenshot(false);
}

void USampleWebPOneShotSubsystem::SampleWebPOneShotCallBackMethod(bool bGenerateWebp)
{
	bWorking = false;
	SampleWebPOneShotCallBackBP.ExecuteIfBound(bGenerateWebp);
}

void USampleWebPOneShotSubsystem::ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors)
{
	if (ScreenHandle.IsValid())
	{
		UGameViewportClient::OnScreenshotCaptured().Remove(ScreenHandle);
	}
	
	AsyncTask(ENamedThreads::AnyThread, [this, InWidth, InHeight, InColors]() {
		FString FileName = FGuid::NewGuid().ToString() + TEXT("_OneShot.webp");
		FString PictruePath = FPaths::ProjectSavedDir() / FileName;
		
		bool bFinishWebp = FSampleWebPCore::GenerateSaticWebpPicture(PictruePath, InColors, FVector2D(InWidth, InHeight), 100);
		//一定要是回调到主线程
		AsyncTask(ENamedThreads::GameThread, [this, bFinishWebp]() {
			SampleWebPOneShotCallBack.ExecuteIfBound(bFinishWebp);
			});
		});
}
