// Fill out your copyright notice in the Description page of Project Settings.


#include "ParryingCheck.h"

void UParryingCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// TODO : player�� IsParrying���� ���� �ϳ� �����ؼ� true��.
}

void UParryingCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// TODO : player�� IsParrying���� ���� �ϳ� �����ؼ� false��.
	// TODO : ������ ������ TakeDamage���� if(IsParrying) -> �и�����.
}