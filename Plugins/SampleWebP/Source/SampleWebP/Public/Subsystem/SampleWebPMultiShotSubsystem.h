#pragma once

#include "CoreMinimal.h"
#include "SampleWebPType.h"
#include "SampleWebPMultiShotSubsystem.generated.h"

struct FSampleWebpPictureInformation;

//Webp的结果生成回调
DECLARE_DELEGATE_OneParam(FSampleWebPMultiCallBack, bool)

UCLASS()
class  USampleWebPMultiShotSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	friend class USampleWebPBPLibrary;

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const  override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override;

	virtual TStatId GetStatId() const override;

protected:

	bool BeginRecord(FString& InGeneratedWebpPicturesPath, TSharedPtr<FSampleWebpPictureInformation> InWebpPictureInformation);

	void EndRecord(FWebpFinishGenerateMultiWebp& InFinshWebpBPDelegate);
	
	void ResetRecord();

	UFUNCTION()
	void ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);

	UFUNCTION()
	void SampleWebPOneMultiCallBackMethod(bool bGenerateWebp);

	ESampleWebpProcessType ProcessType = ESampleWebpProcessType::None;
	FString GeneratedWebpPicturesPath =TEXT("");
	TSharedPtr<FSampleWebpPictureInformation>  WebpPictureInformation;
	TArray<TArray<FColor>> WebPColor;
	TArray<int32> WebpTimestepMillisecond;
	
	FDelegateHandle ScreenHandle;
	FWebpFinishGenerateMultiWebp FinshWebpBPDelegate;
	FSampleWebPMultiCallBack SampleWebPMultiCallBack;

	FCriticalSection WebpMutex;
};

