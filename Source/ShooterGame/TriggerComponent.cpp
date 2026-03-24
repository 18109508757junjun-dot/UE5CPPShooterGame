// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (MoverActor != nullptr)
	{
		Mover = MoverActor->FindComponentByClass<UMoveComponent>();
		if (Mover != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Successfully found the mover component!"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to find the mover component!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("MoverActor is nullptr"));
	}

	// 如果是压力板，绑定重叠事件
	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;
	if (Mover != nullptr)
	{
		Mover->SetMove(NewTriggerValue);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s doesn't have a mover to trigger!"),*GetOwner()->GetActorNameOrLabel());
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		OverlappingActorsNum++;//当有actor进入box时，重叠的actor数量加1

		if (!IsTriggered)//当有其他actor进入box时，它不会重复触发
		{
			Trigger(true);

		}
	}

}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		OverlappingActorsNum--;//当有actor离开box时，重叠的actor数量减1
		if (IsTriggered && OverlappingActorsNum == 0)//当没有actor与box重叠时，触发器才会关闭
		{
			Trigger(false);

		}
	}
}
