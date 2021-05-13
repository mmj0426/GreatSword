// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PhaseCheck.h"
#include "Boss_AIController.h"
#include "Boss.h"
#include "BossStatComponent.h"

#include "behaviorTree/BlackboardComponent.h"


UBTS_PhaseCheck::UBTS_PhaseCheck()
{
	NodeName = TEXT("BTS_PhaseCheck");
	Interval = 1.0f;
}

void UBTS_PhaseCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	//GSLOG(Warning, TEXT("Phase Check"));

	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss == nullptr) return;

	if (Boss->BossStat->GetCurrentHP() > (Boss->BossStat->GetMaxHP() * 0.8))
	{
		Boss->SetCurrentPhase(EBossPhase::Phase1);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ABoss_AIController::PhaseKey,(uint8)EBossPhase::Phase1);
		return ;
	}
	else if (Boss->BossStat->GetCurrentHP() > (Boss->BossStat->GetMaxHP() * 0.4))
	{
		Boss->SetCurrentPhase(EBossPhase::Phase2);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ABoss_AIController::PhaseKey, (uint8)EBossPhase::Phase2);
		return ;
	}
	else
	{
		Boss->SetCurrentPhase(EBossPhase::Phase3);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ABoss_AIController::PhaseKey, (uint8)EBossPhase::Phase3);
		return ;
	}
}