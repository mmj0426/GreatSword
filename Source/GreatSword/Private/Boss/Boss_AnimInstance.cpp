// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AnimInstance.h"

#include "Boss.h"
#include "BossStatComponent.h"

#include "UObject/ConstructorHelpers.h"

UBoss_AnimInstance::UBoss_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	BasicAttack_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Boss_Attack01.Boss_Attack01"));
	if (BasicAttack_Montage.Succeeded())
	{
		BasicAttackMontage = BasicAttack_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Death_Montage (TEXT("/Game/Blueprints/Enemies/Animation/Boss_Death.Boss_Death"));
	if (Death_Montage.Succeeded())
	{
		DeathMontage = Death_Montage.Object;
	}
}

void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* const Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(TryGetPawnOwner()))
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
	}
}

void UBoss_AnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(BasicAttackMontage))
	{
		Montage_Play(BasicAttackMontage,1.0f);
		CurrentState = EBossState::Attacking;
	}
}

void UBoss_AnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage,1.0f);
		CurrentState = EBossState::Death;
	}
}