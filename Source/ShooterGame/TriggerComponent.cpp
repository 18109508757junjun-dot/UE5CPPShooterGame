// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 新用法：填多个 Actor（例如双开门左右门扇）
	Movers.Reset();

	for (AActor* Actor : MoverActors)//遍历MoverActors数组中的每个actor
	{
		if (!Actor)//如果actor为空，则跳过
		{
			continue;
		}
		if (UMoveComponent* FoundMover = Actor->FindComponentByClass<UMoveComponent>())//如果actor有MoveComponent组件，则添加到Movers数组中
		{
			Movers.Add(FoundMover);
		}
		else//如果actor没有MoveComponent组件，则打印错误信息
		{
			UE_LOG(LogTemp, Display, TEXT("%s doesn't have a MoveComponent!"), *Actor->GetActorNameOrLabel());
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Trigger controls %d mover(s)."), Movers.Num());//打印Movers数组中的数量

	// 如果是压力板，绑定重叠事件
	if (IsPressurePlate)//如果是压力板，绑定重叠事件
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);//绑定重叠开始事件
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);//绑定重叠结束事件
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Movers.Num() > 0)//如果Movers数组中的数量大于0，则遍历Movers数组中的每个MoveComponent组件
	{
		for (UMoveComponent* EachMover : Movers)//遍历Movers数组中的每个MoveComponent组件
		{
			if (IsValid(EachMover))//如果MoveComponent组件不为空，则设置移动状态
			{
				EachMover->SetMove(NewTriggerValue);//设置移动状态
			}
		}
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s doesn't have a mover to trigger!"), *GetOwner()->GetActorNameOrLabel());//打印错误信息
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("PressurePlateActivator"))//如果OtherActor不为空，并且OtherActor有PressurePlateActivator标签，则重叠的actor数量加1
	{
		OverlappingActorsNum++;//重叠的actor数量加1

		if (!IsTriggered)//如果IsTriggered为false，则触发器状态为false
		{
			Trigger(true);//触发器状态为true

		}
	}

}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("PressurePlateActivator"))//如果OtherActor不为空，并且OtherActor有PressurePlateActivator标签，则重叠的actor数量减1
	{
		OverlappingActorsNum--;//重叠的actor数量减1
		if (IsTriggered && OverlappingActorsNum == 0)//如果IsTriggered为true，并且重叠的actor数量为0，则触发器状态为false
		{
			Trigger(false);//触发器状态为false

		}
	}
}
