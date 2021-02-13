// Fill out your copyright notice in the Description page of Project Settings.


#include "GSAnimInstance.h"
#include "PlayerCharacter.h"

UGSAnimInstance::UGSAnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	//Attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Attack_Montage(TEXT("/Game/Blueprints/Animations/AnimMontage_AttackCombo.AnimMontage_AttackCombo"));
	if (Attack_Montage.Succeeded())
	{
		AttackMontage = Attack_Montage.Object;
	}

	//Smash
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Smash_Montage(TEXT("/Game/Blueprints/Animations/AnimMontage_Smash.AnimMontage_Smash"));
	if (Smash_Montage.Succeeded())
	{
		SmashMontage = Smash_Montage.Object;
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
	GSCHECK(!Montage_IsPlaying(AttackMontage));
	GSCHECK(!Montage_IsPlaying(SmashMontage));
	Montage_Play(AttackMontage, 1.0f);
}

void UGSAnimInstance::PlaySmashMontage()
{
	GSLOG(Warning, TEXT("AnimInstance : PlaySmashMontage"));
	GSCHECK(!Montage_IsPlaying(SmashMontage));
	Montage_Play(SmashMontage,1.0f);
}

void UGSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	GSCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection),AttackMontage);
}

void UGSAnimInstance::JumpToSmashMontageSection(int32 NewSection)
{
	GSCHECK(Montage_IsPlaying(SmashMontage));
	Montage_JumpToSection(GetSmashMontageSectionName(NewSection), SmashMontage);
}

void UGSAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UGSAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UGSAnimInstance::AnimNotify_SmashCheck()
{
	OnSmashCheck.Broadcast();
}

FName UGSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	GSCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"),Section));
}

FName UGSAnimInstance::GetSmashMontageSectionName(int32 Section)
{
	GSCHECK(FMath::IsWithinInclusive<int32>(Section,2,4),NAME_None);
	return FName(*FString::Printf(TEXT("Smash%d"),Section));
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
	GSCHECK(!Montage_IsPlaying(ParryingMontage));
	Montage_Play(ParryingMontage, 1.0f);
}

void UGSAnimInstance::PlayDodgeMontage()
{
	GSCHECK(!Montage_IsPlaying(DodgeMontage));
	GSCHECK(!Montage_IsPlaying(AttackMontage));
	GSCHECK(!Montage_IsPlaying(SmashMontage));
	Montage_Play(DodgeMontage, 1.0f);
}