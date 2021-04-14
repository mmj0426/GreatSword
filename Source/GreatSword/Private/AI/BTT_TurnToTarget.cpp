// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToTarget.h"

#include "Boss_AIController.h"
#include "Boss.h"
#include "PlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_TurnToTarget::UBTT_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Boss)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABoss_AIController::TargetKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	// Turn to Target
	FVector LookVector = Target->GetActorLocation() - Boss->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Boss->SetActorRotation(FMath::RInterpTo(Boss->GetActorRotation(),TargetRot,GetWorld()->GetDeltaSeconds(),2.0f));

	return EBTNodeResult::Succeeded;
}