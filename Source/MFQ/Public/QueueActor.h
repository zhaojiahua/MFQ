// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QueueActor.generated.h"

UCLASS()
class MFQ_API AQueueActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQueueActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "zjhAddAttrs")
		TArray<class AProgressActor*> progressArray;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,category="zjhAddAttrs")
		int32 queueLevel = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "zjhAddAttrs")
		int32 timeSlice = 1;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, category = "zjhAddFuns")
		void AddPro(class AProgressActor* inPro);
	UFUNCTION(BlueprintCallable, category = "zjhAddFuns")
		void RemovePro(class AProgressActor* inPro);
	UFUNCTION(BlueprintCallable, category = "zjhAddFuns")
		void KillPro(class AProgressActor* inPro);

};
