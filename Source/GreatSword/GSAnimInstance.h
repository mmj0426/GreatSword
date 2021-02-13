// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "GSAnimInstance.generated.h"

/**
 * 
 */

 DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
 DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
 DECLARE_MULTICAST_DELEGATE(FOnSmashCheckDelegate);
 DECLARE_MULTICAST_DELEGATE(FOnParryingEndDelegate);
 DECLARE_MULTICAST_DELEGATE(FOnDodgeEndDelegate);

UCLASS()
class GREATSWORD_API UGSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public : 
	UGSAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//Attack Combo
	void PlayAttackMontage();

	void PlaySmashMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	void JumpToSmashMontageSection(int32 NewSection);

	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	FOnSmashCheckDelegate OnSmashCheck;

	// Evade 
	FOnParryingEndDelegate OnParryingEnd;
	FOnDodgeEndDelegate OnDodgeEnd;

	void PlayParryingMontage();

	void PlayDodgeMontage();
	

private : 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	//!< Legacy

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	//bool IsParrying;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	//bool IsDodge;

	// Attack

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SmashMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ParryingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_SmashCheck();
	
	FName GetAttackMontageSectionName(int32 Section);

	FName GetSmashMontageSectionName(int32 Section);

	// Evade

	//!< Legacy
	//UFUNCTION()
	//void AnimNotify_ParryingEnd();

	//UFUNCTION()
	//void AnimNotify_DodgeEnd();

};
