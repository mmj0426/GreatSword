// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS(Blueprintable)
class GREATSWORD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Weapon;

	UPROPERTY()
	class UGSAnimInstance* GSAnim;

	//Attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	// Evade

	//!< Legacy
	
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	//bool IsParrying;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
	//bool IsDodge;

private:

	//Movement
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void Run();
	void Walk();

	//Attack
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupeted);

	void Attack();

	//공격 속성 지정
	void AttackStartComboState();
	void AttackEndComboState();

	//Evade
	void Evade();

public : 

	//!< Legacy
	//bool GetIsParrying() const { return IsParrying;}
	//bool GetIsDodge() const{return IsDodge; }
	
};
