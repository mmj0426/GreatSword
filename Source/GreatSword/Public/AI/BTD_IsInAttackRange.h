// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBTD_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public : 
	UBTD_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};
