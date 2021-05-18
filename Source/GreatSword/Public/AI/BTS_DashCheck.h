// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTService.h"
#include "BTS_DashCheck.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBTS_DashCheck : public UBTService
{
	GENERATED_BODY()
public : 
	UBTS_DashCheck();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
