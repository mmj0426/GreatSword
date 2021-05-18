// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_DashCheck.h"
#include "Boss_AIController.h"
#include "Boss.h"
#include "PlayerCharacter.h"

#include "behaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTS_DashCheck::UBTS_DashCheck()
{
	NodeName = TEXT("BTS_DashCheck");
	Interval = 1.0f;
}

void UBTS_DashCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 1200.0f;

	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam);


	auto Boss = Cast<ABoss>(OwnerComp.GetAIOwner()->GetPawn());

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			APlayerCharacter* Player_Character = Cast<APlayerCharacter>(OverlapResult.GetActor());

			if (Player_Character)
			{

				/*OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABoss_AIController::TargetKey, Player_Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Player_Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Player_Character->GetActorLocation(), FColor::Blue, false, 0.27f);*/
				return;
			}
		}
	}
	else
	{
		Boss->CanDash = true;
		//DrawDebugPoint(World, Player_Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
		/*OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABoss_AIController::TargetKey, nullptr);
		GSLOG(Warning, TEXT("Target Is Not Set"));*/
	}

	//OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABoss_AIController::TargetKey, nullptr);
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
