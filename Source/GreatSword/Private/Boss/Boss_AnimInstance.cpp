// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AnimInstance.h"

#include "Boss.h"
#include "BossStatComponent.h"

#include "UObject/ConstructorHelpers.h"

UBoss_AnimInstance::UBoss_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
	Death_Montage (TEXT("/Game/Blueprints/Enemies/Animation/AnimMontage_Death.AnimMontage_Death"));
	if (Death_Montage.Succeeded())
	{
		DeathMontage = Death_Montage.Object;
	}
}

void UBoss_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* const Boss = Cast<ABoss>(TryGetPawnOwner());
	if (::IsValid(TryGetPawnOwner()))
	{
		CurrentPawnSpeed = TryGetPawnOwner()->GetVelocity().Size();
	}
}

void UBoss_AnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		GSLOG(Warning, TEXT("Playing"));
		Montage_Play(DeathMontage,1.0f);
	}
}