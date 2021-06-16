// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "Gothic_AnimInstance.generated.h"

UENUM(BlueprintType)
enum class EGothicState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attacking UMETA(DisplayName = "Attacking"),
	Death UMETA(DisplayName = "Death")
};
/**
 * 
 */
UCLASS()
class GREATSWORD_API UGothic_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public : 
	UGothic_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	void PlayAttackMontage(int32 MontageIndex);

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_AttackStart();

private : 
	UPROPERTY(VisibleAnywhere, Category = Attack)
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	EGothicState CurrentState;

private : 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack01;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack02;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack03;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack04;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Phase1_Attack05;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

public : 
	void SetCurrentState(EGothicState newState) { CurrentState = newState; }
	EGothicState GetCurrentState(){return CurrentState;}
};
