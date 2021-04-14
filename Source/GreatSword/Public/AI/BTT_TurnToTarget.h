// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBTT_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public : 
	UBTT_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
