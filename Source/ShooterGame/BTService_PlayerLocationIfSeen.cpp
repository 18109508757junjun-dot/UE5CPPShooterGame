// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());//将行为树组件的AI所有者转换为AShooterAI类型，并赋值给OwnerController变量。
	AShooterGameCharacter* Player = OwnerController->PlayerCharacter;//从OwnerController中获取玩家角色，并赋值给Player变量。
	UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();//从OwnerController中获取黑板组件，并赋值给Blackboard变量。

	if (OwnerController && Player && Blackboard)
	{
		if (OwnerController->LineOfSightTo(Player))
		{
			OwnerController->SetFocus(Player);//设置AI的焦点为玩家角色，这样AI就会一直关注玩家角色的位置和状态。
			
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());//GetSelectedBlackboardKey()的作用是获取黑板组件中选定的键名，这个键名是在编辑器中设置的，这里是PlayerLocation。
		}
		else
		{
			Blackboard->ClearValue(GetSelectedBlackboardKey());//消除关键值playerlocation，当ai失去该值时，便会进入下一个调查阶段
			OwnerController->ClearFocus(EAIFocusPriority::Gameplay);//消除ai视线追踪
		}
	}
}
