// Fill out your copyright notice in the Description page of Project Settings.


#include "GSAnimInstance.h"

UGSAnimInstance::UGSAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
}

void UGSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}