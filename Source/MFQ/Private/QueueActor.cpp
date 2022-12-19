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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, inPro->proColor, inPro->GetProcName() + FString(TEXT(" add to ")) + FString::FromInt(queueLevel));
	inPro->queueAge = (inPro->mTaskLife - inPro->mPassAge);//发生队列的转移时queueAge一定要更新(queueAge用于计算队列进程移位)(表示它刚进此级队列时的所剩寿命长度)
	inPro->queue2Timer = 0;
	inPro->queue3Timer = 0;
	inPro->queue4Timer = 0;
	int32 i = 0;
	for (auto iPro : progressArray) {
		i += iPro->queueAge;
	}
	FVector3d queueLocation = GetActorLocation();
	queueLocation.Y -= 100 * (i + inPro->queueAge);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, inPro->proColor, FString(TEXT("i ")) + FString::FromInt(i) + FString(TEXT(" queueLocation.Y ")) + FString::SanitizeFloat(queueLocation.Y));
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
			temploc.Y += 100*inPro->queueAge;
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
			temploc.Y += 100*inPro->queueAge;
			progressArray[i]->SetActorLocation(temploc);
		}
	}
	progressArray.Remove(inPro);	//保险起见先充数组中删除后再销毁
	inPro->KillSelf();
}

