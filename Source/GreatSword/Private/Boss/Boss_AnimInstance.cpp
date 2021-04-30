// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AnimInstance.h"

#include "Boss.h"
#include "BossStatComponent.h"

#include "UObject/ConstructorHelpers.h"

UBoss_AnimInstance::UBoss_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BasicAttack_Montage01(TEXT("/Game/Blueprints/Enemies/Animation/Attack/BasicAttack01.BasicAttack01"));
	if (BasicAttack_Montage01.Succeeded())
	{
		BasicAttackMontage01 = BasicAttack_Montage01.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		BasicAttack_Montage02(TEXT("/Game/Blueprints/Enemies/Animation/Attack/BasicAttack02.BasicAttack02"));
	if (BasicAttack_Montage02.Succeeded())
	{
		BasicAttackMontage02 = BasicAttack_Montage02.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Death_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Boss_Dead.Boss_Dead"));
	if (Death_Montage.Succeeded())
	{
		DeathMontage = Death_Montage.Object;
	}

	BossAttackMontages.Emplace(TEXT("Phase1_Attack01"),BasicAttackMontage01);
	BossAttackMontages.Emplace(TEXT("Phase1_Attack02"),BasicAttackMontage02);
	
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

void UBoss_AnimInstance::PlayAttackMontage(FString MontageName)
{
	if (!Montage_IsPlaying(BossAttackMontages[MontageName]))
	{
		Montage_Play(BossAttackMontages[MontageName],1.0f);
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