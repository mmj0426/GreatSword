// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BasicAttack01.generated.h"


/**
 *
 */
UCLASS()
class GREATSWORD_API UBTT_BasicAttack01 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BasicAttack01();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking;
};
