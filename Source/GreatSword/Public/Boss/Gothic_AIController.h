// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "AIController.h"
#include "Gothic_AIController.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API AGothic_AIController : public AAIController
{
	GENERATED_BODY()
public : 
	AGothic_AIController();
	virtual void OnPossess(APawn* InPawn)override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName IsAliveKey;

private:

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class AGothic* GSGothic;
};
