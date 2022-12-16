// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgressActor.h"

// Sets default value
AProgressActor::AProgressActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ZjhRootComp"));
	RootComponent = rootComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(rootComp);
	subMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("subMeshComp"));
	subMeshComp->SetupAttachment(meshComp);

	mTaskLife = FMath::RandRange(1, 20);
	proColor = FColor(FMath::RandRange(10, 255), FMath::RandRange(10, 255), FMath::RandRange(10, 255), 255);
	processID = FMath::RandRange(100, 10000);
	queueAge = mTaskLife;	//进程一开始的时候总是要进第一级队列,它刚进第一级队列的时候的生命长度就是它原始的寿命长度

}



// Called when the game starts or when spawned
void AProgressActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProgressActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProgressActor::KillSelf()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, proColor, GetProcName() + FString(TEXT("been Killed")));
	Destroy();
}

FString AProgressActor::GetProcName()
{
	return FString(TEXT("pro:")) + FString::FromInt(processID);
}

void AProgressActor::ExecutePro(float intime)
{
	FVector3d tempLoc = meshComp->GetRelativeScale3D();
	tempLoc.Y -= intime;
	meshComp->SetRelativeScale3D(tempLoc);
}
void AProgressActor::StopPro()
{
	beginClock = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, proColor, GetProcName() + FString(TEXT("stop")));
}


