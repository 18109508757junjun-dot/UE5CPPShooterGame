// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//创建一个默认的场景组件，并将其设置为根组件
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	//创建一个默认的骨骼网格组件，并将其附加到根组件上
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(SceneRoot);
	//创建一个默认的Niagara组件，并将其附加到枪的骨骼网格组件上
	MuzzleFlashParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlash"));
	MuzzleFlashParticles->SetupAttachment(GunMesh);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	MuzzleFlashParticles->Deactivate();//在游戏开始时，停用枪口闪光粒子系统，使其不显示
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger()
{
	MuzzleFlashParticles->Activate(true);//当玩家按下扳机时，激活枪口闪光粒子系统，使其显示
	//UE_LOG(LogTemp, Display, TEXT("bang!"));
	if (GunOwner) 
	{
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		GunOwner->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);//获取枪的拥有者的视点位置和旋转


		FHitResult HitResult;

		FVector EndLocation = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;//计算线性追踪的终点位置，起点位置加上视点旋转的前方向量乘以枪的最大射程
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);//将枪本身添加到线性追踪的忽略列表中，避免线性追踪检测到枪自己
		Params.AddIgnoredActor(GetOwner());//将枪的拥有者添加到线性追踪的忽略列表中，避免线性追踪检测到枪的拥有者
		bool IsHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ViewPointLocation,
			EndLocation,
			ECC_GameTraceChannel1, Params
		);//在游戏世界中进行一次线性追踪，起点为枪的拥有者的视点位置，终点为视点位置加上视点旋转的前方向量乘以1000，使用可见性通道，结果存储在HitResult中

		if (IsHit && ImpactParticlesSystem)
		{
			//在命中时，如果ImpactParticlesSystem不为空，则在命中位置生成一个粒子系统，使用UNiagaraFunctionLibrary的SpawnSystemAtLocation函数，传入游戏世界、ImpactParticlesSystem、命中位置和命中位置的旋转作为参数
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticlesSystem, HitResult.ImpactPoint,HitResult.ImpactPoint.Rotation());
		}
	}
	
}

