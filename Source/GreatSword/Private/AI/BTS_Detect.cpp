// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect.h"
#include "Boss_AIController.h"
#include "Boss.h"

#include "behaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTS_Detect::UBTS_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if(nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None,false,ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
	OverlapResults,
	Center,
	FQuat::Identity,
	ECollisionChannel::ECC_EngineTraceChannel2,
	FCollisionShape::MakeSphere(DetectRadius),
	CollisionQueryParam);

	DrawDebugSphere(World,Center,DetectRadius,16,FColor::Red,false,0.2f);

}