// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"

#include "Boss.generated.h"

UENUM(Category = BossState)
enum class EBossState : uint8
{
	Death
};

UCLASS()
class GREATSWORD_API ABoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Boss)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UBossStatComponent* BossStat;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Boss, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

private : 
	// AnimInstance
	UPROPERTY()
	class UBoss_AnimInstance* BossAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EBossState CurrentState;
};
