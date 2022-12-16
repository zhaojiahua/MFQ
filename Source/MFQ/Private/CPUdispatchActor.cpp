
#include "CPUdispatchActor.h"
#include "ProgressActor.h"
#include "QueueActor.h"

// Sets default values
ACPUdispatchActor::ACPUdispatchActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPUdispatchActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPUdispatchActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mIncreaseTimeUnit += DeltaTime;
	if (queueActor1->progressArray.Num() > 0) {//如果第一级队列不为空就取出其首元素反复执行直到时间片达到1(第一级队列所给的时间片)
		auto iPro = queueActor1->progressArray[0];
		iPro->ExecutePro(DeltaTime);
		if (mIncreaseTimeUnit >= 1) {	//一个时间片执行结束
			mIncreaseTimeUnit = 0.0f;
			iPro->mPassAge += 1;	//passAge增加1
			//此时要开始判断此进程的执行程度
			if (iPro->mPassAge >= iPro->mTaskLife) {	//如果进程的年龄大于或等于它的总寿命说明执行完成了,应该被销毁了
				queueActor1->KillPro(iPro);
			}
			else {	//走到这里说明年龄没有大于寿命,这个进程在第一级队列完成了一个时间轮转要进入下一级队列(的队尾)
				queueActor1->RemovePro(iPro);
				queueActor2->AddPro(iPro);
				iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;	//发生队列的转移时queueAge一定要更新(queueAge用于计算队列进程移位)(表示它刚进二级队列时所剩的生命长度)
			}
		}
	}
	/// /// ///二级队列 ///
	else if (queueActor2->progressArray.Num() > 0) {	//如果第一级队列空了才开始执行第二级队列(中间如果有新的进程任务出现在第一级队列,这时候要立刻停止此队列的执行,转而执行第一级队列)
		auto iPro = queueActor2->progressArray[0];	//同样取出首位置元素
		iPro->ExecutePro(DeltaTime);
		if (mIncreaseTimeUnit >= 1) {	//一个时间片执行结束
			mIncreaseTimeUnit = 0.0f;
			iPro->queue2Timer++;	//标识在此进程在队列2中已经被执行了一个时间片了
			iPro->mPassAge += 1;	//passAge增加1
			//此时要开始判断此进程的执行程度
			if (iPro->mPassAge >= iPro->mTaskLife) {	//如果进程的年龄大于或等于它的总寿命说明执行完成了,应该被销毁了
				queueActor2->KillPro(iPro);
			}
			else if (iPro->queue2Timer < queueActor2->timeSlice) {	//在执行到此时刻如果一级队列有新进程了,就要在这个时候中断执行,并把它放入此队列队尾(并且要重置queue2Timer=0)(并且还要更新queueAge)
				if (queueActor1->progressArray.Num() > 0) {
					queueActor2->RemovePro(iPro);
					queueActor2->AddPro(iPro);
					iPro->queue2Timer = 0;
					iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;
				}
			}
			else if (iPro->queue2Timer >= queueActor2->timeSlice) {	//如果此时已经经过了两个时间片而且来到这里说明还没有执行完成,那么就需要转移到三级队列(队尾)
				queueActor2->RemovePro(iPro);
				queueActor3->AddPro(iPro);
				iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;	//发生队列的转移时queueAge一定要更新(queueAge用于计算队列进程移位)(表示它刚进三级队列时的所剩寿命长度)
			}
		}
	}
	/// /// ///三级队列 ///
	else if (queueActor3->progressArray.Num() > 0) {	//如果第二级队列空了才开始执行第三级队列(中间如果出现新的进程要中断,转而执行更高级别的队列)
		auto iPro = queueActor3->progressArray[0];	//同样取出首位置元素
		iPro->ExecutePro(DeltaTime);
		if (mIncreaseTimeUnit >= 1) {	//一个时间片执行结束
			mIncreaseTimeUnit = 0.0f;
			iPro->queue3Timer++;	//标识在此进程在队列3中已经被执行了一个时间片了
			iPro->mPassAge ++;	//passAge增加1

			//此时要开始判断此进程的执行程度
			if (iPro->mPassAge >= iPro->mTaskLife) {	//如果进程的年龄大于或等于它的总寿命说明执行完成了,应该被销毁了
				queueActor3->KillPro(iPro);
			}
			else if (iPro->queue3Timer < queueActor3->timeSlice) {	//在执行到此时刻如果一二级队列有新进程了,就要在这个时候中断执行,并把它放入此队列队尾(并且要重置queue3Timer=0)(并且还要更新queueAge)
				if (queueActor1->progressArray.Num() > 0 || queueActor2->progressArray.Num() > 0) {
					queueActor3->RemovePro(iPro);
					queueActor3->AddPro(iPro);
					iPro->queue3Timer = 0;
					iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;
				}
			}
			else if (iPro->queue3Timer >= queueActor3->timeSlice) {	//如果此时已经经过了4个时间片而且来到这里说明还没有执行完成,那么就需要转移到三级队列(队尾)
				queueActor3->RemovePro(iPro);
				queueActor4->AddPro(iPro);
				iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;	//发生队列的转移时queueAge一定要更新(queueAge用于计算队列进程移位)(表示它刚进三级队列时的所剩寿命长度)
			}
		}
	}
	/// /// ///四级队列 ///
	else if (queueActor4->progressArray.Num() > 0) {	//第一 二 三级队列全部空了才开始执行第几四队列(如果第四级队列执行一遍后仍有未完成的进程就将该进程放置带此队尾,在第一二三级队列仍然为空的情况重复执行此队列的进程,知道清空)
		auto iPro = queueActor4->progressArray[0];	//同样取出首位置元素
		iPro->ExecutePro(DeltaTime);
		if (mIncreaseTimeUnit >= 1) {	//一个时间片执行结束
			mIncreaseTimeUnit = 0.0f;
			iPro->queue4Timer++;	//标识此进程在队列4中已经被执行了一个时间片了
			iPro->mPassAge++;	//passAge增加1
			//此时要开始判断此进程的执行程度
			if (iPro->mPassAge >= iPro->mTaskLife) {	//如果进程的年龄大于或等于它的总寿命说明执行完成了,应该被销毁了
				queueActor4->KillPro(iPro);
			}
			else if (queueActor1->progressArray.Num() > 0 || queueActor2->progressArray.Num() > 0 || queueActor3->progressArray.Num() > 0 || iPro->queue4Timer >= queueActor4->timeSlice) {	//在执行到此时刻如果一二三级队列有新进程了,就要在这个时候中断执行,并把它放入此队列队尾(并且要重置queue4Timer=0)(并且还要更新queueAge)
					queueActor4->RemovePro(iPro);
					queueActor4->AddPro(iPro);
					iPro->queue4Timer = 0;
					iPro->queueAge = iPro->mTaskLife - iPro->mPassAge;
				}
			
		}
	}
}


