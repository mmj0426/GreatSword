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

UCLASS()
class GREATSWORD_API UGSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public : 
	UGSAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//Attack Combo
	void PlayAttackMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnNextAttackCheckDelegate OnNextAttackCheck;

private : 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = ture))
		bool IsParrying;


	// Attack

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	
	FName GetAttackMontageSectionName(int32 Section);

};
