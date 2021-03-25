// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"
#include "NavigationSystem.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ABoss_AIController::HomePosKey(TEXT("HomePos"));
const FName ABoss_AIController::PatrolPosKey(TEXT("PatrolPos"));

ABoss_AIController::ABoss_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
	BB_Object(TEXT("/Game/Blueprints/Enemies/AI/BB_Boss.BB_Boss"));

	if (BB_Object.Succeeded())
	{
		BBAsset = BB_Object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
	BT_Object(TEXT("/Game/Blueprints/Enemies/AI/BT_Boss.BT_Boss"));

	if (BT_Object.Succeeded())
	{
		BTAsset = BT_Object.Object;
	}
}

void ABoss_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{	
		Blackboard->SetValueAsVector(HomePosKey,InPawn->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			GSLOG(Warning,TEXT("AI Controller couldn't run behavior tree"));
		}
	}
}
