// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BasicAttack.h"

#include "Boss_AIController.h"
#include "Boss.h"

UBTT_BasicAttack::UBTT_BasicAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTT_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss) 
	{
		return EBTNodeResult::Failed;
	}

	Boss->Attack();
	IsAttacking = true;

	Boss->OnAttackEnd.AddLambda([this]()-> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTT_BasicAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp,NodeMemory,DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	}

	//FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
}
