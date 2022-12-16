#include "QueueActor.h"
#include "ProgressActor.h"

// Sets default values
AQueueActor::AQueueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	for (auto iPro : progressArray) {
		FVector3d queueLocation = GetActorLocation();
		int32 posY = iPro->mTaskLife - iPro->mPassAge;
		iPro->SetActorLocation(FVector3d(queueLocation.X, posY, queueLocation.Z));
	}
}

// Called when the game starts or when spawned
void AQueueActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQueueActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQueueActor::AddPro(AProgressActor* inPro)
{
	GEngine->AddOnScreenDebugMessage(0, 5.0f, inPro->proColor, inPro->GetProcName() + FString(TEXT(" add to ")) + FString::FromInt(queueLevel));
	int32 i = 0;
	for (auto iPro : progressArray) {
		i += (iPro->mTaskLife - iPro->mPassAge);
	}
	FVector3d queueLocation = GetActorLocation();
	queueLocation.Y -= 100 * (i + (inPro->mTaskLife - inPro->mPassAge));
	inPro->SetActorLocation(queueLocation);
	progressArray.Add(inPro);
}

void AQueueActor::RemovePro(AProgressActor* inPro)
{
	int32 proIndex = progressArray.IndexOfByKey(inPro);
	//如果移除的不是队尾的话,那么被移除后后面的Pro都要往前挤
	if (proIndex != progressArray.Num() - 1) {
		for (int32 i = proIndex; i < progressArray.Num(); i++) {
			FVector3d temploc = progressArray[i]->GetActorLocation();
			temploc.Y -= inPro->queueAge;
			temploc.Y *= 100;
			progressArray[i]->SetActorLocation(temploc);
		}
	}
	progressArray.Remove(inPro);
}
void AQueueActor::KillPro(AProgressActor* inPro)
{
	int32 proIndex = progressArray.IndexOfByKey(inPro);
	//如果杀死的不是队尾的进程的话,那么被移除后后面的进程都要往前挤
	if (proIndex != progressArray.Num() - 1) {
		for (int32 i = proIndex; i < progressArray.Num(); i++) {
			FVector3d temploc = progressArray[i]->GetActorLocation();
			temploc.Y -= inPro->queueAge;
			temploc.Y *= 100;
			progressArray[i]->SetActorLocation(temploc);
		}
	}
	progressArray.Remove(inPro);	//保险起见先充数组中删除后再销毁
	inPro->KillSelf();
}

