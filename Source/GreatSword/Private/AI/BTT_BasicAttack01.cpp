// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BasicAttack01.h"

#include "Boss_AIController.h"
#include "Boss.h"

UBTT_BasicAttack01::UBTT_BasicAttack01()
{
	NodeName = TEXT("BasicAttack01");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTT_BasicAttack01::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Boss)
	{
		return EBTNodeResult::Failed;
	}


	Boss->BossAttack();

	IsAttacking = true;

	Boss->OnAttackEnd.AddLambda([this]()-> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTT_BasicAttack01::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	//FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
}
