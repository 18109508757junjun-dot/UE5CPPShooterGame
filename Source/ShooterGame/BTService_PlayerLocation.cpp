// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Get Player Location");

}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());//将行为树组件的AI所有者转换为AShooterAI类型，并赋值给OwnerController变量。
	AShooterGameCharacter* Player = OwnerController->PlayerCharacter;//从OwnerController中获取玩家角色，并赋值给Player变量。
	UBlackboardComponent* Blackboard = OwnerController->GetBlackboardComponent();//从OwnerController中获取黑板组件，并赋值给Blackboard变量。

	if (OwnerController && Player && Blackboard)
	{
		Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
	}
	
}
