// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AnimInstance.h"

#include "Boss.h"
#include "BossStatComponent.h"

#include "UObject/ConstructorHelpers.h"

UBoss_AnimInstance::UBoss_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Front01.Front01"));
	if (Phase1_Attack01_Montage.Succeeded())
	{
		Phase1_Attack01 = Phase1_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Ground01.Ground01"));
	if (Phase1_Attack02_Montage.Succeeded())
	{
		Phase1_Attack02 = Phase1_Attack02_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Front02.Front02"));
	if (Phase2_Attack01_Montage.Succeeded())
	{
		Phase2_Attack01 = Phase2_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Ground01-2.Ground01-2"));
	if (Phase2_Attack02_Montage.Succeeded())
	{
		Phase2_Attack02 = Phase2_Attack02_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack03_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/L_Foot.L_Foot"));
	if (Phase2_Attack03_Montage.Succeeded())
	{
		Phase2_Attack03 = Phase2_Attack03_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack04_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/R_Foot.R_Foot"));
	if (Phase2_Attack04_Montage.Succeeded())
	{
		Phase2_Attack04 = Phase2_Attack04_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase2_Attack05_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Jump.Jump"));
	if (Phase2_Attack05_Montage.Succeeded())
	{
		Phase2_Attack05 = Phase2_Attack05_Montage.Object;
	}	
	
	//static ConstructorHelpers::FObjectFinder<UAnimMontage>
	//	Phase2_Attack06_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Dash.Dash"));
	//if (Phase2_Attack06_Montage.Succeeded())
	//{
	//	Phase2_Attack06 = Phase2_Attack06_Montage.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase3_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Front03.Front03"));
	if (Phase3_Attack01_Montage.Succeeded())
	{
		Phase3_Attack01 = Phase3_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase3_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Attack/Ground02.Ground02"));
	if (Phase3_Attack02_Montage.Succeeded())
	{
		Phase3_Attack02 = Phase3_Attack02_Montage.Object;
	}

	BossAttackMontages.Emplace(TEXT("Front01"),Phase1_Attack01);
	BossAttackMontages.Emplace(TEXT("Ground01"),Phase1_Attack02);

	BossAttackMontages.Emplace(TEXT("Front02"), Phase2_Attack01);
	BossAttackMontages.Emplace(TEXT("Ground01-2"), Phase2_Attack02);
	BossAttackMontages.Emplace(TEXT("L_Foot"), Phase2_Attack03);
	BossAttackMontages.Emplace(TEXT("R_Foot"), Phase2_Attack04);
	BossAttackMontages.Emplace(TEXT("Jump"), Phase2_Attack05);
	//BossAttackMontages.Emplace(TEXT("Dash"), Phase2_Attack06);

	BossAttackMontages.Emplace(TEXT("Front03"),Phase3_Attack01);
	BossAttackMontages.Emplace(TEXT("Ground02"), Phase3_Attack02);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Death_Montage(TEXT("/Game/Blueprints/Enemies/Animation/Boss_Dead.Boss_Dead"));
	if (Death_Montage.Succeeded())
	{
		DeathMontage = Death_Montage.Object;
	}
			
	IsDead = false;
}

void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* const Boss = Cast<ABoss>(TryGetPawnOwner());
	if (! ::IsValid(TryGetPawnOwner())) return;
	if(!IsDead)
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
	}
	
}

void UBoss_AnimInstance::PlayAttackMontage(FString MontageName)
{
	GSCHECK(!IsDead);
	if (!Montage_IsPlaying(BossAttackMontages[MontageName]))
	{
		Montage_Play(BossAttackMontages[MontageName],1.0f);
		CurrentState = EBossState::Attacking;
	}
}

void UBoss_AnimInstance::PlayDeathMontage()
{
	GSCHECK(!IsDead);
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage,1.0f);
		CurrentState = EBossState::Death;
	}
}

//void UBoss_AnimInstance::AnimNotify_AttackHitCheck()
//{
//	
//}
