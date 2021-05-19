// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacterStatComponent.generated.h"


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
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public : 
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxHP;
	UPROPERTY(EditAnywhere, Category = Stat)
	float MaxStamina;

	float GetCurrentHP() { return CurrentHP; }
	void SetCurrentHP(float HP) { CurrentHP = HP; }
	float GetMaxHP() { return MaxHP; }

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

};
