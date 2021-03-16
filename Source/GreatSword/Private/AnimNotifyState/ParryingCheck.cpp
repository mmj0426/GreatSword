// Fill out your copyright notice in the Description page of Project Settings.


#include "ParryingCheck.h"

void UParryingCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// TODO : player에 IsParrying같은 변수 하나 설정해서 true로.
}

void UParryingCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// TODO : player에 IsParrying같은 변수 하나 설정해서 false로.
	// TODO : 나머지 로직은 TakeDamage에서 if(IsParrying) -> 패링성공.
}