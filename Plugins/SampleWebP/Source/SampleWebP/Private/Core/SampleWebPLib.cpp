#include "SampleWebPLib.h"
#include <algorithm>

#include "demux.h"
#include "encode.h"
#include "mux.h"


bool FSampleWebPLib::GenerateWebpByRGBA(const char* InWebpSavePath, const unsigned char* InRGBAData, int InWidth,
                                        int InHeight, int InQuality)
{
	if (!PLATFORM_WINDOWS)	return false;
	InQuality = std::min(InQuality, 100);
	InQuality = std::max(InQuality, 0);

	uint8_t* Output = nullptr;
	size_t Size = WebPEncodeRGBA(InRGBAData, InWidth, InHeight, InWidth * 4, InQuality, &Output);
	if (Size == 0)	return false;

	FILE* FDes = fopen(InWebpSavePath, "wb");
	if (FDes)
	{
		fwrite(Output, Size, 1, FDes);
		fclose(FDes);
		WebPFree(Output);
	}
	else
	{
		WebPFree(Output);
		return false;
	}
	
	return true;
}

bool FSampleWebPLib::GenerateDymaicWebpByRGBA(const char* InWebpSavePath,
	std::vector<const unsigned char*>& InRGBADatas, std::vector<int> InTimestamps_ms, int InWidth, int InHeight,
	float InQualityFactor)
{
	if (!PLATFORM_WINDOWS) return false;
	if (InTimestamps_ms.size() != InRGBADatas.size()) return false;
	InQualityFactor = std::min(InQualityFactor, 100.0f);
	InQualityFactor = std::max(InQualityFactor, 0.0f);

	WebPConfig WebpPictureConfig;
	if (!WebPConfigInit(&WebpPictureConfig))  return false;
	WebpPictureConfig.lossless = 0;
	WebpPictureConfig.method = 4;
	WebpPictureConfig.quality = InQualityFactor;

	WebPAnimEncoderOptions EncodeOptions;
	if (!WebPAnimEncoderOptionsInit(&EncodeOptions))  return false;
	EncodeOptions.kmin = 0;
	EncodeOptions.kmax = 1;

	WebPAnimEncoder* Encoder = WebPAnimEncoderNew(InWidth, InHeight, &EncodeOptions);

	float TimePoint = 0.f;
	auto DataItem = InRGBADatas.begin();
	auto TimeStepItem = InTimestamps_ms.begin();

	for (; DataItem != InRGBADatas.end(); ++DataItem, ++TimeStepItem)
	{
		WebPPicture WebpOneFramePicture;
		WebPPictureInit(&WebpOneFramePicture);

		WebpOneFramePicture.use_argb = 1;
		WebpOneFramePicture.width = InWidth;
		WebpOneFramePicture.height = InHeight;
		WebpOneFramePicture.argb_stride = InWidth * 4;

		WebPPictureImportRGBA(&WebpOneFramePicture, *DataItem, InWidth * 4);
		TimePoint += *TimeStepItem;
		WebPAnimEncoderAdd(Encoder, &WebpOneFramePicture, TimePoint, &WebpPictureConfig);
		WebPPictureFree(&WebpOneFramePicture);
	}
	WebPAnimEncoderAdd(Encoder, NULL, TimePoint, NULL);

	WebPData WebpPictureData;
	WebPAnimEncoderAssemble(Encoder, &WebpPictureData);
	WebPAnimEncoderDelete(Encoder);

	FILE* FDes = fopen(InWebpSavePath, "wb");
	if (FDes)
	{
		fwrite(WebpPictureData.bytes, WebpPictureData.size, 1, FDes);
		fclose(FDes);
		WebPDataClear(&WebpPictureData);
		return true;
	}
	else
	{
		WebPDataClear(&WebpPictureData);
		return false;
	}
	
	return true;
}

bool FSampleWebPLib::LoadDynamicWebpPictureByRGBA(const char* InWebpFilePath,
	std::vector<const unsigned char*>& OutRGBADatas, std::vector<int>& OutTimestamps_ms, int& OutWidth, int& OutHeight)
{
	if (!PLATFORM_WINDOWS) return false;
	
	FILE* FFileSource = fopen(InWebpFilePath, "rb");
	if (!FFileSource)  return false;

	//简而言之就是在内存准备一段长度的空间,把磁盘上的文件写到这块空间
	unsigned char* FileOriginalData;
	unsigned int   FileLength;
	fseek(FFileSource, 0, SEEK_END);
	FileLength = ftell(FFileSource);
	FileOriginalData = (unsigned char*)WebPMalloc(FileLength);
	memset(FileOriginalData, 0, FileLength);
	fseek(FFileSource, 0, SEEK_SET);
	fread(FileOriginalData, FileLength, 1, FFileSource);
	fclose(FFileSource);

	WebPData OriginWebpData = { FileOriginalData, (size_t)FileLength};

	WebPAnimDecoderOptions DecOptions;
	WebPAnimDecoderOptionsInit(&DecOptions);

	WebPAnimDecoder* Decoder = WebPAnimDecoderNew(&OriginWebpData, &DecOptions);
	WebPAnimInfo     AnimInfo;
	WebPAnimDecoderGetInfo(Decoder, &AnimInfo);

	OutWidth = AnimInfo.canvas_width;
	OutHeight = AnimInfo.canvas_height;

	OutRGBADatas.clear();
	OutTimestamps_ms.clear();

	int framNum = 0;
	int Timestamp = 0;

	while (WebPAnimDecoderHasMoreFrames(Decoder))
	{
		uint8_t* buf;

		WebPAnimDecoderGetNext(Decoder, &buf, &Timestamp);
		framNum++;

		unsigned char* oneWebpColorData = (unsigned char*)malloc(OutWidth * OutHeight * 4);

		memset(oneWebpColorData, 0, OutWidth * OutHeight * 4);
		memcpy(oneWebpColorData, buf, OutWidth * OutHeight * 4);

		OutRGBADatas.push_back(oneWebpColorData);
		OutTimestamps_ms.push_back(Timestamp);
	}
	WebPAnimDecoderReset(Decoder);
	WebPAnimDecoderDelete(Decoder);
	WebPDataClear(&OriginWebpData);

	return true;
}
