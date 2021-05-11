// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"
#include "Boss.h"
#include "NavigationSystem.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

const FName ABoss_AIController::HomePosKey(TEXT("HomePos"));
const FName ABoss_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABoss_AIController::TargetKey(TEXT("Target"));
const FName ABoss_AIController::IsAliveKey(TEXT("IsAlive"));
const FName ABoss_AIController::PhaseKey(TEXT("Phase"));

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

	GSBoss = Cast<ABoss>(InPawn);
	GSBoss->GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	if (UseBlackboard(BBAsset, Blackboard))
	{	
		Blackboard->SetValueAsVector(HomePosKey,InPawn->GetActorLocation());
		Blackboard->SetValueAsBool(IsAliveKey,true);

		if (!RunBehaviorTree(BTAsset))
		{
			GSLOG(Warning,TEXT("AI Controller couldn't run behavior tree"));
		}
	}
}
