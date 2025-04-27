#include "SampleWebpShowMultiSubsystem.h"

#include "SampleWebPCore.h"

bool USampleWebpShowMultiSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void USampleWebpShowMultiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USampleWebpShowMultiSubsystem::Deinitialize()
{
	ReleaseLoadedWebp();
	Super::Deinitialize();
}

void USampleWebpShowMultiSubsystem::Tick(float DeltaTime)
{
	if (LoadAndShowStatus == ESampleWebpLoadAndShowType::Showing && WebpTexture)
	{
		DeltaTime *= 1000;
		//TODO,cost much
		for (int32 WebpIndex = 0; WebpIndex < WebpTimestepMillisecond.Num(); WebpIndex++)
		{
			if (WebpIndex == 0 && CurrentMillisecond < WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond += DeltaTime;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (WebpIndex == WebpTimestepMillisecond.Num() - 1 && CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond = 0;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				continue;
			}

			CurrentMillisecond += DeltaTime;
			WebpShowIndex = WebpIndex;
			break;
		}

		uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		FMemory::Memcpy(textureData, PicturesColors[WebpShowIndex].GetData(), WebpWidth * WebpHeight * 4);

		WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		WebpTexture->UpdateResource();
	}
}

bool USampleWebpShowMultiSubsystem::IsTickable() const
{
	return !IsTemplate();
}

TStatId USampleWebpShowMultiSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UXGSampleWebpShowMultiSubsystem, STATGROUP_Tickables);
}

void USampleWebpShowMultiSubsystem::LoadWebp(FWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath)
{
	bool bRightPath = FSampleWebPCore::CheckWebpPicturePath(InWebpFilePath);
	bool bRgihtStatus = LoadAndShowStatus == ESampleWebpLoadAndShowType::None;
	if (!bRightPath || !bRgihtStatus)
	{
		InLoadAndShowWebpDegelete.ExecuteIfBound(false, nullptr, 0, 0);
		return;
	}

	LoadAndShowStatus = ESampleWebpLoadAndShowType::Loading;
	WebpLoadAndShowWebp = InLoadAndShowWebpDegelete;

	AsyncTask(ENamedThreads::AnyThread,[InWebpFilePath,this]()
	{
		bool bLoadWebp = FSampleWebPCore::LoadDynamicWebpPicture(InWebpFilePath, WebpTimestepMillisecond, PicturesColors, WebpWidth, WebpHeight);

		AsyncTask(ENamedThreads::GameThread, [bLoadWebp, this]()
		{
			LoadedWebpCallShow(bLoadWebp);
		});
	});
}

void USampleWebpShowMultiSubsystem::ReleaseLoadedWebp()
{
	switch (LoadAndShowStatus)
	{
	case ESampleWebpLoadAndShowType::None:
		break;
	case ESampleWebpLoadAndShowType::Loading:

		AsyncTask(ENamedThreads::GameThread, [this]() {

			ReleaseLoadedWebp();

			});
		
		break;
	case ESampleWebpLoadAndShowType::Showing:
		LoadAndShowStatus = ESampleWebpLoadAndShowType::None;
		WebpTexture = nullptr;
		WebpShowIndex = -1;
		CurrentMillisecond = -1;
		WebpWidth = 0;
		WebpHeight = 0;
		WebpTimestepMillisecond.Empty();

		for (auto& Tmp : PicturesColors)
		{
			Tmp.Empty();
		}
		PicturesColors.Empty();



		break;
	case ESampleWebpLoadAndShowType::Max:
		break;
	default:
		break;
	}
}

void USampleWebpShowMultiSubsystem::LoadedWebpCallShow(bool bLoaded)
{
	if (!bLoaded)
	{
		LoadAndShowStatus = ESampleWebpLoadAndShowType::None;
		WebpLoadAndShowWebp.ExecuteIfBound(false, nullptr, 0, 0);
		return;
	}

	LoadAndShowStatus = ESampleWebpLoadAndShowType::Showing;

	WebpTexture = UTexture2D::CreateTransient(WebpWidth, WebpHeight, EPixelFormat::PF_R8G8B8A8);
	uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	FMemory::Memcpy(textureData, PicturesColors[0].GetData(), WebpWidth * WebpHeight * 4);
	WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	WebpTexture->UpdateResource();
	
	WebpShowIndex = 0;
	WebpLoadAndShowWebp.ExecuteIfBound(true, WebpTexture, WebpWidth, WebpHeight);
}
