// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Blueprint/UserWidget.h"
#include "Widget_StatBar.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API UWidget_StatBar : public UUserWidget
{
	GENERATED_BODY()
public : 
	void SetPlayerHPPercent(float currentHP);
	void SetPlayerStaminaPercent(float currentStamina);
	void SetBossHPPercent(float currentHP);

protected : 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Player_HP_Percent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Player_Stamina_Percent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Boss_HP_Percent;

	virtual void NativeConstruct() override;
};
