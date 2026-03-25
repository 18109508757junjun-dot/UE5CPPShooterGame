// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 设置tag
	Tags.Add(FName(TEXT("Lock")));

	// 创建根组件
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// 创建触发器组件
	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("TriggerComp"));
	if (IsValid(RootComp) && IsValid(TriggerComp))
	{
		TriggerComp->SetupAttachment(RootComp);
	}

	// 创建静态网格组件用于显示钥匙外观
	KeyItemComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyItemComp"));
	if (IsValid(RootComp) && IsValid(KeyItemComp))
	{
		KeyItemComp->SetupAttachment(RootComp);
	}


}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	SetIsKeyPlaced(false);

}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALock::GetIsKeyPlaced()
{
	return IsKeyPlaced;
}

void ALock::SetIsKeyPlaced(bool NewIsKeyPlaced)
{
	IsKeyPlaced = NewIsKeyPlaced;

	TriggerComp->Trigger(NewIsKeyPlaced);
	KeyItemComp->SetVisibility(NewIsKeyPlaced);
}
