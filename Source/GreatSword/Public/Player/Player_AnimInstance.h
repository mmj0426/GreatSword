// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GreatSword.h"
#include "Animation/AnimInstance.h"
#include "Containers/Array.h"

#include "Player_AnimInstance.generated.h"

UENUM(Category = State)
enum class EPlayerState
{
	Idle,
	//Moving,
	Attacking,
	Parrying,
	Dodge
};

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

	//void PlayMontage(EPlayerState playerState);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		EPlayerState CurrentPlayerState;


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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Parrying, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ParryingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Dodge, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DodgeMontage;

	// Montage Array 

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	//TMap<EPlayerState, TArray<UAnimMontage*>> PlayerMontageMap;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> AttackMontageArray;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Parrying, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> ParryingMontageArray;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Dodge, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> DodgeMontageArray;

	// Attack Notify
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	UFUNCTION()
		void AnimNotify_SmashCheck();

	FName GetSmashMontageSectionName(int32 Section);

	// Evade


public:
	int32 GetCurrentCombo() const { return CurrentCombo; }
	int32 GetMaxSection() const { return MaxSection; }
	int32 GetCurrentAttackIndex() const { return CurrentAttackIndex; }
	int32 GetCurrentSectionIndex() const { return CurrentSectionIndex; }

	void SetCurrentCombo(int32 NewCurrentCombo) { CurrentCombo = NewCurrentCombo; }

	EPlayerState GetCurrentPlayerState() const { return CurrentPlayerState; }
	void SetCurrentPlayerState(EPlayerState NewState) { CurrentPlayerState = NewState; }
};
