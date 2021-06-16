// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CanAttack.h"
#include "Gothic.h"
#include "Gothic_AIController.h"
#include "PlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTD_CanAttack::UBTD_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTD_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGothic_AIController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	// 공격 가능 범위 || Dash 공격 가능 범위
	auto Gothic = Cast<AGothic>(OwnerComp.GetAIOwner()->GetPawn());

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 300.0f);

	return bResult;
}
