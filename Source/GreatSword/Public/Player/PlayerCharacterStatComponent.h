// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnPlayerHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREATSWORD_API UPlayerCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void PostInitProperties() override;
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

public : 

	FTimerHandle HP_RecoveryHandle;

	FTimerHandle Stamina_RecoveryHandle;

	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP;
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxStamina;

	float GetCurrentHP() { return CurrentHP; }
	void SetCurrentHP(float HP);
	float GetMaxHP() { return MaxHP; }

	float GetCurrentStamina() {return CurrentStamina;}
	void SetCurrentStamina(float Stamina);
	float GetMaxStamina() {return MaxStamina;}

	FOnPlayerHPIsZeroDelegate OnPlayerHPIsZero;

private : 
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentStamina;
		
	UPROPERTY(Transient, EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Damage;

public : 
	float GetDamage()const{ return Damage; }

	void HP_Recovery();
	void Stamina_Recovery();

};
