// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPUdispatchActor.generated.h"

UCLASS()
class MFQ_API ACPUdispatchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPUdispatchActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		class AQueueActor*queueActor1;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		class AQueueActor* queueActor2;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		class AQueueActor* queueActor3;
	UPROPERTY(EditAnywhere, category = "zjhAddAttrs")
		class AQueueActor* queueActor4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "zjhAddAttrs")
		float runSpeed = 1;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float mIncreaseTimeUnit = 0.0f;	//用来记录时间单位,每当它达到1时就被重新值成0(从而决定了整个程序的运行速度)
};
