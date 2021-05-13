// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AnimInstance.h"

#include "Boss.h"
#include "BossStatComponent.h"

#include "UObject/ConstructorHelpers.h"

UBoss_AnimInstance::UBoss_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase1_AttackMontage01.Phase1_AttackMontage01"));
	if (Phase1_Attack01_Montage.Succeeded())
	{
		Phase1_Attack01 = Phase1_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase1_AttackMontage02.Phase1_AttackMontage02"));
	if (Phase1_Attack02_Montage.Succeeded())
	{
		Phase1_Attack02 = Phase1_Attack02_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase2_AttackMontage01.Phase2_AttackMontage01"));
	if (Phase2_Attack01_Montage.Succeeded())
	{
		Phase2_Attack01 = Phase2_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase2_AttackMontage02.Phase2_AttackMontage02"));
	if (Phase2_Attack02_Montage.Succeeded())
	{
		Phase2_Attack02 = Phase2_Attack02_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack03_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase2_AttackMontage03.Phase2_AttackMontage03"));
	if (Phase2_Attack03_Montage.Succeeded())
	{
		Phase2_Attack03 = Phase2_Attack03_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack04_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase2_AttackMontage04.Phase2_AttackMontage04"));
	if (Phase2_Attack04_Montage.Succeeded())
	{
		Phase2_Attack04 = Phase2_Attack04_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack05_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Phase2_AttackMontage05.Phase2_AttackMontage05"));
	if (Phase2_Attack05_Montage.Succeeded())
	{
		Phase2_Attack05 = Phase2_Attack05_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Death_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Boss_Dead.Boss_Dead"));
	if (Death_Montage.Succeeded())
	{
		DeathMontage = Death_Montage.Object;
	}

	BossAttackMontages.Emplace(TEXT("Phase1_Attack01"),Phase1_Attack01);
	BossAttackMontages.Emplace(TEXT("Phase1_Attack02"),Phase1_Attack02);

	BossAttackMontages.Emplace(TEXT("Phase2_Attack01"), Phase2_Attack01);
	BossAttackMontages.Emplace(TEXT("Phase2_Attack02"), Phase2_Attack02);
	BossAttackMontages.Emplace(TEXT("Phase2_Attack03"), Phase2_Attack03);
	BossAttackMontages.Emplace(TEXT("Phase2_Attack04"), Phase2_Attack04);
	BossAttackMontages.Emplace(TEXT("Phase2_Attack05"), Phase2_Attack05);
															 
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