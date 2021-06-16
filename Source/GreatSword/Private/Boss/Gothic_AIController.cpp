// Fill out your copyright notice in the Description page of Project Settings.


#include "Gothic_AIController.h"
#include "Gothic.h"
#include "NavigationSystem.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

const FName AGothic_AIController::HomePosKey(TEXT("HomePos"));
const FName AGothic_AIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AGothic_AIController::TargetKey(TEXT("Target"));
const FName AGothic_AIController::IsAliveKey(TEXT("IsAlive"));

AGothic_AIController::AGothic_AIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
	BB_Object(TEXT("/Game/Blueprints/Enemies/AI/BB_Gothic.BB_Gothic"));
	if (BB_Object.Succeeded())
	{
		BBAsset = BB_Object.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
	BT_Object(TEXT("/Game/Blueprints/Enemies/AI/BT_Gothic.BT_Gothic"));
	if (BT_Object.Succeeded())
	{
		BTAsset = BT_Object.Object;
	}
}

void AGothic_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GSGothic = Cast<AGothic>(InPawn);
	GSGothic->GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsBool(IsAliveKey, true);

		if (!RunBehaviorTree(BTAsset))
		{
			GSLOG(Warning, TEXT("AI Controller couldn't run behavior tree"));
		}
	}
}