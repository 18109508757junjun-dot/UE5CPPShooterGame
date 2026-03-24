// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERGAME_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere)
	FVector TargetLocation;
	UPROPERTY(VisibleAnywhere)
	FVector StartLocation;
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;
	UPROPERTY(EditAnywhere)
	float MoveTime = 4.0f;


private:
	UPROPERTY(VisibleAnywhere)
	bool bMove = false;

public:
	bool GetMove() const { return bMove; }
	void SetMove(bool NewMove);
};
