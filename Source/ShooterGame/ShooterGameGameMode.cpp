// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameCharacter.h"
#include "ShooterAI.h"

AShooterGameGameMode::AShooterGameGameMode()
{
	// stub
}

void AShooterGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	AShooterGameCharacter* Player = Cast<AShooterGameCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//获取玩家角色，并将其转换为AShooterGameCharacter类型，赋值给Player变量。
	TArray<AActor*> ShooterAIActors;//定义一个AActor类型的数组，用于存储场景中所有的AShooterAI角色。
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterAI::StaticClass(), ShooterAIActors);//获取场景中所有的AShooterAI角色，并将它们存储在ShooterAIActors数组中。
	for (int32 Index = 0; Index < ShooterAIActors.Num(); Index++)
	{
		AActor* ShooterAIActor = ShooterAIActors[Index];
		AShooterAI* ShooterAI = Cast<AShooterAI>(ShooterAIActor);
		if (ShooterAI)
		{
			ShooterAI->StartBehaviorTree(Player);//调用每个AShooterAI角色的StartBehaviorTree函数，并将玩家角色作为参数传递给它。
		}
	}

}
