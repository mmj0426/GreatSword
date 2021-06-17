// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Components/ActorComponent.h"
#include "BossStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnBossHPIsZeroDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GREATSWORD_API UBossStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBossStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitializeComponent()override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetHP(float NewDamage);

	FOnBossHPIsZeroDelegate OnBossHPIsZero;

	FTimerHandle HUDInitHandle;

	float GetCurrentHP() { return CurrentHP; }
	void SetCurrentHP(float HP) { CurrentHP = HP; }
	float GetMaxHP() { return MaxHP; }	

	UPROPERTY(Transient, EditAnywhere, Category = Stat)
	float BaseDamage;

private:
	UPROPERTY(EditAnywhere, Category = Stat)
		float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float CurrentHP;

	

};
