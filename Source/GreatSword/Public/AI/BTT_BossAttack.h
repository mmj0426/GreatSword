// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBTT_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_BossAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking;
};
