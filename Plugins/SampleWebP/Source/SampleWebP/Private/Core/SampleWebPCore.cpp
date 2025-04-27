#include "SampleWebPCore.h"

#include "LogSampleWebP.h"
#include "SampleWebPLib.h"
#include "SampleWebPType.h"


#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"

// 保存为png格式，用于测试
void SaveColorArrayToPng(const TArray<FColor>& InPictureColors, int32 Width, int32 Height, const FString& FilePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	if (ImageWrapper.IsValid() && InPictureColors.Num() == Width * Height)
	{
		ImageWrapper->SetRaw(InPictureColors.GetData(), InPictureColors.Num() * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8);
		const TArray64<uint8>& PngData = ImageWrapper->GetCompressed();
		FFileHelper::SaveArrayToFile(PngData, *FilePath);
	}
}


bool FSampleWebPCore::GenerateSaticWebpPicture(FString InPicturePath, const TArray<FColor>& InPictureColors,
                                                const FVector2D& InPictureSize, int32 InQuality)
{
	SaveColorArrayToPng(InPictureColors, InPictureSize.X, InPictureSize.Y, "E:\\CppU\\LibWebPDemo\\Saved\\1.png");
	UE_LOG(LogTemp, Warning, TEXT("GenerateSaticWebpPicture"));
	if (!CheckWebpPicturePath(InPicturePath))
	{
		return false;
	}
	if (InPictureColors.Num() != InPictureSize.X * InPictureSize.Y)
	{
		UE_LOG(LogSampleWebP, Warning, TEXT("Invalid WebP picture size: %d"), InPictureColors.Num());
	}

	const char* OutPicturePath = TCHAR_TO_ANSI(*InPicturePath);

	unsigned char* Data = new unsigned char[InPictureColors.Num() * 4 + 1];
	int i = 0;
	for (auto& Tmp : InPictureColors)
	{
		Data[i++] = Tmp.R;
		Data[i++] = Tmp.G;
		Data[i++] = Tmp.B;
		Data[i++] = Tmp.A;
	}

	InQuality = FMath::Clamp(InQuality, 0, 100);

	bool Result = FSampleWebPLib::GenerateWebpByRGBA(OutPicturePath, Data, InPictureSize.X, InPictureSize.Y, InQuality);

	delete[] Data;
	Data = nullptr;

	return true;
}

bool FSampleWebPCore::CheckWebpPicturePath(FString& InPicturePath)
{
	FString Extension = FPaths::GetExtension(InPicturePath);
	FString Directory = FPaths::GetPath(InPicturePath);

	if (Extension.Equals(TEXT("webp")) && FPaths::DirectoryExists(Directory))
	{
		return true;
	}
	
	UE_LOG(LogSampleWebP, Warning, TEXT("Invalid WebP picture path: %s"), *InPicturePath);
	return false;
}

bool FSampleWebPCore::GenerateDynamicWebpPicture(FString& InPicturePath,
	TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation, TArray<TArray<FColor>>& InPicturesColors,
	TArray<int32>& WebpTimestepMillisecond, int32 InQualityFactor)
{
	if (!CheckWebpPicturePath(InPicturePath))
		return false;
	if (InPicturesColors.Num() != WebpTimestepMillisecond.Num())
		return false;
	for (auto& InPictureColors : InPicturesColors)
		if (InPictureColors.Num() != InWebpPictureInformation->GetPictureWidth() * InWebpPictureInformation->GetPictureHeight())
			return false;

	const char* OutPicturePath = TCHAR_TO_ANSI(*InPicturePath);
	
	std::vector<const unsigned char*> Datas;
	for (auto& InPictureColors : InPicturesColors)
	{
		unsigned char* Data = new unsigned char[InPictureColors.Num() * 4 + 1];
		int i = 0;
		for (auto& Tmp : InPictureColors)
		{
			Data[i++] = Tmp.R;
			Data[i++] = Tmp.G;
			Data[i++] = Tmp.B;
			Data[i++] = Tmp.A;
		}

		Datas.push_back(Data);
	}

	std::vector<int32> Timestamps(WebpTimestepMillisecond.GetData(), 
							  WebpTimestepMillisecond.GetData() + WebpTimestepMillisecond.Num());
	InQualityFactor = FMath::Clamp(InQualityFactor, 0, 100);

	bool Result = FSampleWebPLib::GenerateDymaicWebpByRGBA(
		OutPicturePath,
		Datas,
		Timestamps,
		InWebpPictureInformation->GetPictureWidth(),
		InWebpPictureInformation->GetPictureHeight(),
		InQualityFactor);

	for (auto& Data : Datas)
	{
		delete[] Data;
		Data = nullptr;
	}

	return Result;
}

bool FSampleWebPCore::GetViewportSize(UObject* WorldContextObject, FVector2D& OutViewportSize)
{
	
	if (WorldContextObject && WorldContextObject->GetWorld())
	{
		if (UGameViewportClient* ViewportClient = WorldContextObject->GetWorld()->GetGameViewport())
		{
			ViewportClient->GetViewportSize(OutViewportSize);
			return true;
		}
	}
	
	return false;
	// if (!WorldContextObject || !WorldContextObject->GetWorld())
	// 	return false;
	//
	// if (UGameViewportClient* InViewportClient = WorldContextObject->GetWorld()->GetGameViewport())
	// {
	// 	if (FSceneViewport* InViewport = InViewportClient->GetGameViewport())
	// 	{
	// 		InViewportClient->GetViewportSize(OutViewportSize);
	//
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

bool FSampleWebPCore::CheckInWebpPictureSize(UObject* WorldContextObject,
	TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
		return false;
	if (FVector2D ViewportSize; GetViewportSize(WorldContextObject, ViewportSize))
	{
		bool bX0 = InWebpPictureInformation->X0 >= 0 && InWebpPictureInformation->X0 < InWebpPictureInformation->X1;
		bool bX1 = InWebpPictureInformation->X1 <= ViewportSize.X-1;
		bool bY0 = InWebpPictureInformation->Y0 >= 0 && InWebpPictureInformation->Y0 < InWebpPictureInformation->Y1;
		bool bY1 = InWebpPictureInformation->Y1 <= ViewportSize.Y-1;
		
		if (bX0 && bX1 && bY0 && bY1)
			return true;
	}
	return false;
}

bool FSampleWebPCore::LoadDynamicWebpPicture(FString InWebpFilePath, TArray<int32>& OutWebpTimestepMillisecond,
	TArray<TArray<FColor>>& OutPicturesColors, int32& OutWebpWidth, int32& OutWebpHeight)
{
	if (!CheckWebpPicturePath(InWebpFilePath)) return false;

	OutWebpTimestepMillisecond.Empty();
	OutPicturesColors.Empty();

	const char* InWebpFilePath_ANSI = TCHAR_TO_ANSI(*InWebpFilePath);
	std::vector<const unsigned char*> OutRGBADatas;
	std::vector<int> OutTimestamps_ms;

	bool bLoadWebp = FSampleWebPLib::LoadDynamicWebpPictureByRGBA(InWebpFilePath_ANSI, OutRGBADatas, OutTimestamps_ms, OutWebpWidth, OutWebpHeight);
	if (!bLoadWebp) return false;

	OutPicturesColors.AddDefaulted(OutRGBADatas.size());
	for (int32 WebpIndex = 0; WebpIndex < OutRGBADatas.size(); WebpIndex++)
	{
		TArray<FColor>& OneWebpColor = OutPicturesColors[WebpIndex];
		OneWebpColor.AddDefaulted(OutWebpWidth * OutWebpHeight);
		FMemory::Memcpy(OneWebpColor.GetData(), OutRGBADatas[WebpIndex], OutWebpWidth * OutWebpHeight * 4);
		
		OutWebpTimestepMillisecond.Add(OutTimestamps_ms[WebpIndex]);
		free((void*)OutRGBADatas[WebpIndex]);
	}
	return true;
}
