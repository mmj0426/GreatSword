// Fill out your copyright notice in the Description page of Project Settings.


#include "Gothic_AnimInstance.h"
#include "Gothic.h"

UGothic_AnimInstance::UGothic_AnimInstance()
{
	CurrentPawnSpeed = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Phase1_Attack01_Montage(TEXT("/Game/Blueprints/Enemies/Tutorial/Phase1_01_Montage.Phase1_01_Montage"));
	if (Phase1_Attack01_Montage.Succeeded())
	{
		Phase1_Attack01 = Phase1_Attack01_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Phase1_Attack02_Montage(TEXT("/Game/Blueprints/Enemies/Tutorial/Phase1_02_Montage.Phase1_02_Montage"));
	if (Phase1_Attack02_Montage.Succeeded())
	{
		Phase1_Attack02 = Phase1_Attack02_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack03_Montage(TEXT("/Game/Blueprints/Enemies/Tutorial/Phase1_03_Montage.Phase1_03_Montage"));
	if (Phase1_Attack03_Montage.Succeeded())
	{
		Phase1_Attack03 = Phase1_Attack03_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack04_Montage(TEXT("/Game/Blueprints/Enemies/Tutorial/Phase1_04_Montage.Phase1_04_Montage"));
	if (Phase1_Attack04_Montage.Succeeded())
	{
		Phase1_Attack04 = Phase1_Attack04_Montage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Phase1_Attack05_Montage(TEXT("/Game/Blueprints/Enemies/Tutorial/Phase1_05_Montage.Phase1_05_Montage"));
	if (Phase1_Attack05_Montage.Succeeded())
	{
		Phase1_Attack05 = Phase1_Attack05_Montage.Object;
	}

	AttackMontages.Add(Phase1_Attack01);
	AttackMontages.Add(Phase1_Attack02);
	AttackMontages.Add(Phase1_Attack03);
	AttackMontages.Add(Phase1_Attack04);
	AttackMontages.Add(Phase1_Attack05);

	IsDead = false;
	
}

void UGothic_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AGothic* const Gothic = Cast<AGothic>(TryGetPawnOwner());
	if (!::IsValid(TryGetPawnOwner())) return;
	if (!IsDead)
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
		IsDead = !(Gothic->IsAlive);
	}
}

void UGothic_AnimInstance::PlayAttackMontage(int32 MontageIndex)
{
	GSCHECK(!IsDead);
	if (!Montage_IsPlaying((AttackMontages[MontageIndex])))
	{
		Montage_Play(AttackMontages[MontageIndex],1.f);
		CurrentState = EGothicState::Attacking;
	}

}
