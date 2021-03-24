// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterStatComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "UObject/Class.h"


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

	//PlayerMontageMap.Add(EPlayerState::Attacking, AttackMontageArray);

	// Parrying
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Parrying_Montage(TEXT("/Game/Blueprints/Player/Animation/Evade/AnimMontage_Parrying.AnimMontage_Parrying"));
	if (Parrying_Montage.Succeeded())
	{
		ParryingMontage = Parrying_Montage.Object;
	}

	ParryingMontageArray.Add(ParryingMontage);
	//PlayerMontageMap.Add(EPlayerState::Parrying, ParryingMontageArray);

	// Dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		Dodge_Montage(TEXT("/Game/Blueprints/Player/Animation/Evade/AnimMontage_Dodge.AnimMontage_Dodge"));
	if (Dodge_Montage.Succeeded())
	{
		DodgeMontage = Dodge_Montage.Object;
	}

	DodgeMontageArray.Add(DodgeMontage);
	//PlayerMontageMap.Add(EPlayerState::Dodge, DodgeMontageArray);
}

void UPlayer_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* const Player = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (::IsValid(TryGetPawnOwner()))
	{
		//CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
		CurrentPawnSpeed = Player->GetVelocity().Size();
		//CurrentAttackIndex = Player->GetAttackComboIndex();
	}
}

//void UPlayer_AnimInstance::PlayMontage(EPlayerState playerState)
//{
//	TArray<UAnimMontage*> CurrentMontage = PlayerMontageMap[playerState];
//
//	if (playerState == EPlayerState::Attacking)
//	{
//		CurrentCombo = 0;
//
//		CurrentSectionIndex = 0;
//
//		MaxSection = AttackMontageArray[CurrentAttackIndex - 1]->CompositeSections.Num() - 1;
//
//		CurrentAttackMontage = AttackMontageArray[CurrentAttackIndex - 1];
//	}
//}


void UPlayer_AnimInstance::PlayAttackMontage(int32 NextCombo)
{
	//GSLOG(Warning, TEXT("PlayAttackMontage : %i"), NextCombo);

	CurrentCombo = 0;
	
	CurrentAttackIndex = NextCombo;
	CurrentSectionIndex = 0;

	//GSLOG(Warning, TEXT("CurrentAttackIndex : %d"), CurrentAttackIndex);
	//GSLOG(Warning, TEXT("CurrentSection : %d"), CurrentSectionIndex);

	MaxSection = AttackMontageArray[CurrentAttackIndex - 1]->CompositeSections.Num()-1;

	CurrentAttackMontage = AttackMontageArray[CurrentAttackIndex - 1];

	if (!Montage_IsPlaying((CurrentAttackMontage)))
	{
		Montage_Play(CurrentAttackMontage, 1.0f);
		CurrentPlayerState = EPlayerState::Attacking;
	}
}


void UPlayer_AnimInstance::PlayParryingMontage()
{
	if (!Montage_IsPlaying(ParryingMontage))
	{
		Montage_Play(ParryingMontage, 1.0f);
		CurrentPlayerState = EPlayerState::Parrying;
	}
}

void UPlayer_AnimInstance::PlayDodgeMontage()
{
	if ((!Montage_IsPlaying(DodgeMontage)))
	{
		Montage_Play(DodgeMontage, 1.0f);
		CurrentPlayerState = EPlayerState::Dodge;
	}
}

void UPlayer_AnimInstance::JumpToSmashMontageSection(int32 NewSection)
{
	CurrentSectionIndex = NewSection;
	//GSLOG(Warning, TEXT("CurrentSection : %d"),CurrentSectionIndex);

	if (Montage_IsPlaying(CurrentAttackMontage))
	{
		Montage_JumpToSection(GetSmashMontageSectionName(NewSection), CurrentAttackMontage);
		CurrentPlayerState = EPlayerState::Attacking;
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