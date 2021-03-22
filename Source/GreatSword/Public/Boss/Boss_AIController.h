// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "AIController.h"
#include "Boss_AIController.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API ABoss_AIController : public AAIController
{
	GENERATED_BODY()
	
public : 
	ABoss_AIController();
	virtual void OnPossess(APawn* InPawn)override;
	virtual void OnUnPossess()override;

private : 
	void OnRepeatTimer();

	FTimerHandle RepeatTimeHandle;
	float RepeatInterval;
};
