// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "ShooterAI.h"
UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
	NodeName = TEXT("Shoot at Player");
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	if (OwnerController)
	{
		AShooterGameCharacter* AICharacter = OwnerController->AICharacter;//获得AI敌人
		AShooterGameCharacter* PlayerCharacter = OwnerController->PlayerCharacter;//获得玩家

		if (AICharacter && PlayerCharacter && PlayerCharacter->IsAlive)
		{
			AICharacter->Shoot();
			Result = EBTNodeResult::Succeeded;
		}
	}
	

	return Result;
}
