// Fill out your copyright notice in the Description page of Project Settings.


#include "GSAnimInstance.h"
#include "PlayerCharacter.h"

UGSAnimInstance::UGSAnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Attack_Montage(TEXT("/Game/Blueprints/Animations/AnimMontage_AttackCombo.AnimMontage_AttackCombo"));
	if (Attack_Montage.Succeeded())
	{
		AttackMontage = Attack_Montage.Object;
	}

	// Parrying
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Parrying_Montage(TEXT("/Game/Blueprints/Animations/AnimMontage_Parrying.AnimMontage_Parrying"));
	if (Parrying_Montage.Succeeded())
	{
		ParryingMontage = Parrying_Montage.Object;
	}

	// Dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Dodge_Montage(TEXT("/Game/Blueprints/Animations/AnimMontage_Dodge.AnimMontage_Dodge"));
	if (Dodge_Montage.Succeeded())
	{
		DodgeMontage = Dodge_Montage.Object;
	}
}

void UGSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* const Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (::IsValid(TryGetPawnOwner()))
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();

		//IsParrying = Player->GetIsParrying();
		//IsDodge = Player->GetIsDodge();
	}
}

void UGSAnimInstance::PlayAttackMontage()
{
	GSLOG(Warning, TEXT("AnimInstance : Play Attack Montage"));
	Montage_Play(AttackMontage, 1.0f);
}

void UGSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	GSCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection),AttackMontage);
}

void UGSAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UGSAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UGSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	GSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"),Section));
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

void UGSAnimInstance::PlayParryingMontage()
{
	if (!Montage_IsPlaying(ParryingMontage))
	{
		Montage_Play(ParryingMontage, 1.0f);
	}
}

void UGSAnimInstance::PlayDodgeMontage()
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		Montage_Play(DodgeMontage, 1.0f);
	}
}