// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInAttackRange.h"
#include "Boss.h"
#include "Boss_AIController.h"
#include "PlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsInAttackRange::UBTD_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTD_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp,NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn) 
	{
		return false;
	}

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABoss_AIController::TargetKey));
	if(nullptr == Target)
	{
		return false;
	}
	
	// 공격 가능 범위 || Dash 공격 가능 범위
	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (Boss->GetCurrentPhase() != EBossPhase::Phase1)
	{
		bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f || Target->GetDistanceTo(ControllingPawn) >= 1200.0f);
		if (Target->GetDistanceTo(ControllingPawn) >= 1200.0f)
		{
			Boss->CanDash = true;
		}
		return bResult;
	}
	else
	{
		bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f);
		return bResult;
	}

}
