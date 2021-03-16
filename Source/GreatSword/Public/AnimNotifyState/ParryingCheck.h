// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ParryingCheck.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UParryingCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public : 

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
