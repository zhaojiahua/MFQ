// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProgressActor.generated.h"

UCLASS()
class MFQ_API AProgressActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProgressActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		USceneComponent* rootComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, category = "zjhAddAttrs")
		UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,category = "zjhAddAttrs")
		UStaticMeshComponent* subMeshComp;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		UMaterialInterface* matInterface;
	UMaterialInstanceDynamic* dynamicMat;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		class USoundCue* soundForKill;
	int32 mPriority = 7;	//线程优先级默认给7(生成线程的时候随机给一个1-7的值)

	//线程的运行总时长(默认给4个单位的时间片)
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, category = "zjhAddAttrs")
		int32 mTaskLife = 4;
	UPROPERTY(BlueprintReadOnly, category = "zjhAddAttrs")
		int32 mPassAge = 0;	//线程任务已经执行的时长
	int32 mQueue = 0;	//当前所在队列(队列从1-4,四个级别的队列)
	int32 queueAge=0;	//用于记录线程刚进入队列时的长度
	int32 queue2Timer = 0;	//用于记录在二级队列中被执行的时间片数
	int32 queue3Timer = 0;	//用于记录在三级队列中被执行的时间片数
	int32 queue4Timer = 0;	//用于记录在三级队列中被执行的时间片数
	UPROPERTY(BlueprintReadWrite, category = "zjhAddAttrs")
		FColor proColor;
	UFUNCTION(BlueprintCallable,category="zjhAddFuns")
		FString GetProcName();
	UFUNCTION(BlueprintCallable, category = "zjhAddFuns")
		void SetInitialize();
	bool beginClock = false;
	void ExecutePro(float intime);
	void KillSelf();
private:
	float mIncreaseTimer = 0.0f;
	int32 processID = 0;
	
};
