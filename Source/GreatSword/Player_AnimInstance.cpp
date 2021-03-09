// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterStatComponent.h"


UPlayer_AnimInstance::UPlayer_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	//Attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Attack_Montage_01(TEXT("/Game/Blueprints/Player/Animation/Attack/AnimMontage_Attack1.AnimMontage_Attack1"));
	if (Attack_Montage_01.Succeeded())
	{
		AttackMontage_01 = Attack_Montage_01.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Attack_Montage_02(TEXT("/Game/Blueprints/Player/Animation/Attack/AnimMontage_Attack2.AnimMontage_Attack2"));
	if (Attack_Montage_02.Succeeded())
	{
		AttackMontage_02 = Attack_Montage_02.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Attack_Montage_03(TEXT("/Game/Blueprints/Player/Animation/Attack/AnimMontage_Attack3.AnimMontage_Attack3"));
	if (Attack_Montage_03.Succeeded())
	{
		AttackMontage_03 = Attack_Montage_03.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Attack_Montage_04(TEXT("/Game/Blueprints/Player/Animation/Attack/AnimMontage_Attack4.AnimMontage_Attack4"));
	if (Attack_Montage_04.Succeeded())
	{
		AttackMontage_04 = Attack_Montage_04.Object;
	}

	AttackMontageArray.Add(AttackMontage_01);
	AttackMontageArray.Add(AttackMontage_02);
	AttackMontageArray.Add(AttackMontage_03);
	AttackMontageArray.Add(AttackMontage_04);

	// Parrying
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Parrying_Montage(TEXT("/Game/Blueprints/Player/Animation/Evade/AnimMontage_Parrying.AnimMontage_Parrying"));
	if (Parrying_Montage.Succeeded())
	{
		ParryingMontage = Parrying_Montage.Object;
	}

	// Dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Dodge_Montage(TEXT("/Game/Blueprints/Player/Animation/Evade/AnimMontage_Dodge.AnimMontage_Dodge"));
	if (Dodge_Montage.Succeeded())
	{
		DodgeMontage = Dodge_Montage.Object;
	}
}

void UPlayer_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* const Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (::IsValid(TryGetPawnOwner()))
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
		IsAttacking = Player->GetIsAttacking();

		IsParrying = Player->GetIsParrying();
		IsDodge = Player->GetIsDodge();
	}
}

void UPlayer_AnimInstance::PlayAttackMontage(int32 NextCombo)
{
	//GSLOG(Warning, TEXT("PlayAttackMontage : %i"), NextCombo);

	CurrentCombo = 1;
	MaxSection = AttackMontageArray[NextCombo - 1]->CompositeSections.Num();

	CurrentAttackMontage = AttackMontageArray[NextCombo - 1];

	if (!Montage_IsPlaying((CurrentAttackMontage)))
	{
		Montage_Play(CurrentAttackMontage, 1.0f);
	}
}


void UPlayer_AnimInstance::JumpToSmashMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(CurrentAttackMontage))
	{
		Montage_JumpToSection(GetSmashMontageSectionName(NewSection), CurrentAttackMontage);
	}
}

FName UPlayer_AnimInstance::GetSmashMontageSectionName(int32 Section)
{
	GSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 2), NAME_None);
	return FName(*FString::Printf(TEXT("Smash%d"), Section));
}

void UPlayer_AnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UPlayer_AnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UPlayer_AnimInstance::AnimNotify_SmashCheck()
{
	OnSmashCheck.Broadcast();
}

//!< Legacy
//! 
//void UGSAnimInstance::AnimNotify_ParryingEnd()
//{
//	OnParryingEnd.Broadcast();
//}
//
//void UGSAnimInstance::AnimNotify_DodgeEnd()
//{
//	OnDodgeEnd.Broadcast();
//}

void UPlayer_AnimInstance::PlayParryingMontage()
{
	if (!Montage_IsPlaying(ParryingMontage))
	{
		Montage_Play(ParryingMontage, 1.0f);
	}
}

void UPlayer_AnimInstance::PlayDodgeMontage()
{
	if ((!Montage_IsPlaying(DodgeMontage)))
	{
		Montage_Play(DodgeMontage, 1.0f);
	}
}