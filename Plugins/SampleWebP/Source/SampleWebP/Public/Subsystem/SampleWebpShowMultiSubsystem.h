#pragma once

#include "CoreMinimal.h"
#include "SampleWebPType.h"
#include "SampleWebpShowMultiSubsystem.generated.h"

UCLASS()
class USampleWebpShowMultiSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
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
	void LoadWebp(FWebpLoadAndShowWebp InLoadAndShowWebpDegelete, FString InWebpFilePath);
	void ReleaseLoadedWebp();
	void LoadedWebpCallShow(bool bLoaded);

	

	ESampleWebpLoadAndShowType LoadAndShowStatus = ESampleWebpLoadAndShowType::None;

	FWebpLoadAndShowWebp WebpLoadAndShowWebp;

	int32 WebpShowIndex = -1;
	int32 CurrentMillisecond = -1;

	TArray<int32> WebpTimestepMillisecond;
	TArray<TArray<FColor>> PicturesColors;
	UPROPERTY()
    UTexture2D* WebpTexture = nullptr;

	int32 WebpWidth = 0;
	int32 WebpHeight = 0;
};
