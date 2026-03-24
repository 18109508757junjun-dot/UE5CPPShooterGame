// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	// 初始化 StartLocation 为拥有者的当前位置
	StartLocation = GetOwner()->GetActorLocation();

	// 设置初始状态为不移动
	SetMove(false);
	StartLocation = GetOwner()->GetActorLocation();

	// 设置初始状态为不移动
	SetMove(false);
	// 可以在这里初始化或检查组件状态
	// ...
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 获取当前 Actor 的位置
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	// 如果未到达目标位置，则按指定速度插值移动
	if (!CurrentLocation.Equals(TargetLocation))
	{
		// 计算插值速度，移动速率 = 位移长度 / 总用时
		float Speed = MoveOffset.Length() / MoveTime;

		// 计算新位置，将当前位置以常速插值向目标位置推进
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);

		// 设置新的 Actor 位置
		GetOwner()->SetActorLocation(NewLocation);
	}
	// ...
}

// 切换移动状态并更新目标位置
void UMoveComponent::SetMove(bool NewMove)
{
	bMove = NewMove;

	// 若设置为移动，目标位置为起点加移动偏移，否则目标为起点位置
	if (bMove)
	{
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		TargetLocation = StartLocation;
	}
}
