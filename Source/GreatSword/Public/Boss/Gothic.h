// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"
#include "Gothic.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class GREATSWORD_API AGothic : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGothic();

	FOnAttackEndDelegate OnAttackEnd;

	FTimerHandle DelayHandle;
	FTimerHandle WidgetInitHandle;

	void GothicAttack();
	int32 DecideAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents()override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupeted);

	// AnimInstance
	UPROPERTY()
	class UGothic_AnimInstance* GothicAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	bool IsAlive;

	bool IsOddNumber;

	void SetCurrentHP(float damage);
};
