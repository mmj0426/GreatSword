// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IsInDashRange.h"
#include "Boss.h"
#include "Boss_AIController.h"
#include "PlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTD_IsInDashRange::UBTD_IsInDashRange()
{
	NodeName = TEXT("CanDash");
}

bool UBTD_IsInDashRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABoss_AIController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1200.0f);
	if (bResult)
	{
		auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
		Boss->CanDash = true;
	}
	return bResult;
}
