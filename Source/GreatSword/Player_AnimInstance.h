// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "Containers/Array.h"

#include "Player_AnimInstance.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSmashCheck);

DECLARE_MULTICAST_DELEGATE(FOnParryingEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDodgeEndDelegate);

UCLASS()
class GREATSWORD_API UPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayer_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//Attack Combo
	void PlayAttackMontage(int32 NextCombo);
	void JumpToSmashMontageSection(int32 NewSection);

	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnSmashCheck OnSmashCheck;

	// Evade 
	void PlayParryingMontage();
	void PlayDodgeMontage();

	FOnParryingEndDelegate OnParryingEnd;
	FOnDodgeEndDelegate OnDodgeEnd;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool IsParrying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool IsDodge;

	// Attack

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage_01;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage_02;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage_03;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage_04;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> AttackMontageArray;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CurrentAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxSection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentAttackIndex;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentSectionIndex;


	// Evade

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ParryingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DodgeMontage;

	// Attack Notify
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	UFUNCTION()
		void AnimNotify_SmashCheck();

	FName GetSmashMontageSectionName(int32 Section);

	// Evade

	//!< Legacy
	//UFUNCTION()
	//void AnimNotify_ParryingEnd();

	//UFUNCTION()
	//void AnimNotify_DodgeEnd();

public:
	int32 GetCurrentCombo() const { return CurrentCombo; }
	int32 GetMaxSection() const { return MaxSection; }
	int32 GetCurrentAttackIndex() const {return CurrentAttackIndex;}
	int32 GetCurrentSectionIndex() const {return CurrentSectionIndex;}

	void SetCurrentCombo(int32 NewCurrentCombo) { CurrentCombo = NewCurrentCombo; }
};
