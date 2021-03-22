// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ABoss_AIController::ABoss_AIController()
{
	RepeatInterval = 3.0f;
}

void ABoss_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(RepeatTimeHandle, this, &ABoss_AIController::OnRepeatTimer, RepeatInterval, true);
}

void ABoss_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorld()->GetTimerManager().ClearTimer(RepeatTimeHandle);
}

void ABoss_AIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	GSCHECK(nullptr != CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if(nullptr == NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		GSLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
