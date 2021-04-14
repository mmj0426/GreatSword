// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInAttackRange.h"
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

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f);
	return bResult;
	
}
