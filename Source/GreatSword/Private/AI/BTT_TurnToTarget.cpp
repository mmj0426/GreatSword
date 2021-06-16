// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToTarget.h"

#include "Gothic_AIController.h"
#include "Gothic.h"
#include "PlayerCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTT_TurnToTarget::UBTT_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp,NodeMemory);

	auto Gothic = Cast<AGothic>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == Gothic)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AGothic_AIController::TargetKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	// Turn to Target
	FVector LookVector = Target->GetActorLocation() - Gothic->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Gothic->SetActorRotation(FMath::RInterpTo(Gothic->GetActorRotation(),TargetRot,GetWorld()->GetDeltaSeconds(),2.0f));

	return EBTNodeResult::Succeeded;
}