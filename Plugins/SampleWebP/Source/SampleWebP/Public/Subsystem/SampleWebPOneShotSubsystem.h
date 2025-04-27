// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SampleWebPOneShotSubsystem.generated.h"

// Webp生成结果回调
DECLARE_DELEGATE_OneParam(FSampleWebPOneShotCallBack, bool)

//反射到蓝图的生成接口回调
DECLARE_DYNAMIC_DELEGATE_OneParam(FSampleWebPOneShotCallBackBP, bool, bFinishGenerate);

UCLASS()
class SAMPLEWEBP_API USampleWebPOneShotSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION(BlueprintCallable, Category = "SampleWebp|Subsystem")
	void BeginSampleWebPOneShot(FSampleWebPOneShotCallBackBP InSampleWebPOneShotCallBackBP);

protected:

	UFUNCTION()
	void SampleWebPOneShotCallBackMethod(bool bGenerateWebp);

	UFUNCTION()
	void ScreenShotCallback(int32 InWidth, int32 InHeight, const TArray<FColor>& InColors);

	FSampleWebPOneShotCallBack SampleWebPOneShotCallBack;

	FSampleWebPOneShotCallBackBP SampleWebPOneShotCallBackBP;

	FDelegateHandle ScreenHandle;

	bool bWorking =false;
};