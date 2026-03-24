// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MoveComponent.h"
#include "TriggerComponent.generated.h"

/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTriggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
AActor* MoverActor;

UPROPERTY(VisibleAnywhere)
bool IsTriggered = false;

UPROPERTY(EditAnywhere)
bool IsPressurePlate = false;//是否是压力板.

UPROPERTY(VisibleAnywhere)
int32 OverlappingActorsNum = 0;//记录当前与触发器重叠的actor数量


UMoveComponent* Mover;


void Trigger(bool NewTriggerValue);

/** 当某对象进入球体组件时调用 */
UFUNCTION()
void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//OverlappedComp 第一个指针指向与我们重叠的组件 OtherActor第二个指向拥有该组件的actor
/** 当某对象离开球体组件时调用 */
UFUNCTION()
void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
