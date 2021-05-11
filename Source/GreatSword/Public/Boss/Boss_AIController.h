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
	
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName IsAliveKey;
	static const FName PhaseKey;

private : 
	
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class ABoss* GSBoss;

	

};
