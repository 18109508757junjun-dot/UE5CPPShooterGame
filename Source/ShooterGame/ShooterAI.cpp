// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAI.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();

	//PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	

	
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
//优化tips，将StartBehaviorTree放在gamemode里，避免每个AI都调用一次StartBehaviorTree，都去寻找玩家角色，降低性能开销。
void AShooterAI::StartBehaviorTree(AShooterGameCharacter* Player)
{
	if (EnemyAIBehaviorTree)
	{
		AICharacter = Cast<AShooterGameCharacter>(GetPawn());//将AI控制的角色转换为AShooterGameCharacter类型，并赋值给AICharacter变量。
		if (Player)
		{
			PlayerCharacter = Player;
		}
		RunBehaviorTree(EnemyAIBehaviorTree);

		UBlackboardComponent* MyBlackboard = GetBlackboardComponent();
		if (MyBlackboard && PlayerCharacter && AICharacter)
		{
			//MyBlackboard->SetValueAsVector("PlayerLocation", PlayerCharacter->GetActorLocation());//将玩家角色的位置存储在黑板组件中，键名为"PlayerLocation"，值为玩家角色的位置。
			MyBlackboard->SetValueAsVector("AIStartLocation", AICharacter->GetActorLocation());
		}
		
	}
}


