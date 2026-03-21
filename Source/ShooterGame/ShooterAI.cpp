// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAI.h"
#include "Kismet/GameplayStatics.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();

	//PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (EnemyAIBehaviorTree)
	{
		RunBehaviorTree(EnemyAIBehaviorTree);
	}

	
}


// Called every frame
void AShooterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (PlayerPawn)
	{
		if (LineOfSightTo(PlayerPawn))
		{
			SetFocus(PlayerPawn);//设置AI的焦点为玩家角色，这样AI就会一直关注玩家角色的位置和状态。

			MoveToActor(PlayerPawn, 200.0f);//让AI向玩家角色移动，接受半径为200.0f，这意味着当AI距离玩家角色小于200.0f时，就会停止移动。
		}
		else
		{
			ClearFocus(EAIFocusPriority::Gameplay);//EAIFocusPriority::Gameplay这个参数的意思是清除AI的焦点优先级为Gameplay，这样AI就不会再关注玩家角色了。
			StopMovement();//停止AI的移动，这样AI就会停在原地，不再向玩家角色移动了。
		}
		
	}
	*/

}


