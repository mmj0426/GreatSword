// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "Boss_AnimInstance.generated.h"

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
	void PlayDeathMontage();

private : 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnHP;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Death, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
};
