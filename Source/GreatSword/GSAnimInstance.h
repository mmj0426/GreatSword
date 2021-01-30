// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "GSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UGSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public : 
	UGSAnimInstance();

private : 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

public : 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
