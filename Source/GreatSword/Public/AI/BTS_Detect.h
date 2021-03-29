// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Detect.generated.h"

/**
 *
 */
UCLASS()
class GREATSWORD_API UBTS_Detect : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

};
