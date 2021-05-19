// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsInDashRange.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBTD_IsInDashRange : public UBTDecorator
{
	GENERATED_BODY()

public : 
	UBTD_IsInDashRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
