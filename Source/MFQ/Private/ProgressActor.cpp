#include "ProgressActor.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

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
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, proColor, FString(TEXT("------>>>>>>")) + FString::FromInt(mTaskLife));
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
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, proColor, GetProcName() + FString(TEXT(" has been Killed")), true, FVector2D(2.0f, 2.0f));
	UGameplayStatics::PlaySound2D(GetWorld(), soundForKill);
	Destroy();
}

FString AProgressActor::GetProcName()
{
	return FString(TEXT("progress:")) + FString::FromInt(processID);
}


void AProgressActor::SetInitialize()
{
	processID = FMath::RandRange(1000, 90000);
	mTaskLife = FMath::RandRange(1, 20);
	queueAge = mTaskLife;
	proColor = FColor(FMath::RandRange(10, 200), FMath::RandRange(10, 200), FMath::RandRange(10, 200), 255);
	meshComp->SetRelativeScale3D(FVector3d(1.0f, (mTaskLife - mPassAge), 1.0f));
	dynamicMat = UMaterialInstanceDynamic::Create(matInterface, this);
	dynamicMat->SetVectorParameterValue(TEXT("ProgressMatColor"), FLinearColor(proColor));
	subMeshComp->SetMaterial(0, dynamicMat);
}

void AProgressActor::ExecutePro(float intime)
{
	FVector3d tempLoc = meshComp->GetRelativeScale3D();
	tempLoc.Y -= intime;
	meshComp->SetRelativeScale3D(tempLoc);
}

