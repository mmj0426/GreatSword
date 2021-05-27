// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

UENUM(Category = BossState)
enum class EBossState : uint8
{
	Idle,
	Attacking,
	Death
};

/**
 * 
 */
UCLASS()
class GREATSWORD_API UBoss_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public : 
	UBoss_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

;public : 
	void PlayAttackMontage(FString MontageName);
	void PlayDeathMontage();


private : 
	UPROPERTY(VisibleAnywhere, category = Attack)
	TMap<FString, UAnimMontage*> BossAttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EBossState CurrentState;

private : 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack01;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack02;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase2_Attack01;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase2_Attack02;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase2_Attack03;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase2_Attack04;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase2_Attack05;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//UAnimMontage* Phase2_Attack06;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase3_Attack01;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase3_Attack02;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

private : 
	// Anim Notify 

	//UFUNCTION()
	//void AnimNotify_AttackHitCheck();

public : 
	EBossState GetCurrentBossState()const { return CurrentState; }
};
