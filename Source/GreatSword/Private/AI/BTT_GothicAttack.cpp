// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GothicAttack.h"

#include "Gothic_AIController.h"
#include "Gothic.h"

UBTT_GothicAttack::UBTT_GothicAttack()
{
	NodeName = TEXT("Gothic Attack");
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTT_GothicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Gothic = Cast<AGothic>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Gothic)
	{
		return EBTNodeResult::Failed;
	}

	Gothic->GothicAttack();

	IsAttacking = true;

	Gothic->OnAttackEnd.AddLambda([this]()-> void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTT_GothicAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

