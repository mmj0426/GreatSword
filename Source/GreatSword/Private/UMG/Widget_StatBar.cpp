// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_StatBar.h"
#include "PlayerCharacterStatComponent.h"

void UWidget_StatBar::SetPlayerHPPercent(float currentHP)
{
	//float currentHP = CharacterStat->GetCurrentHP();
	Player_HP_Percent = currentHP;
}

void UWidget_StatBar::SetPlayerStaminaPercent(float currentStamina)
{
	Player_Stamina_Percent = currentStamina;
}

void UWidget_StatBar::SetBossHPPercent(float currentHP)
{
	Boss_HP_Percent = currentHP;
}

void UWidget_StatBar::NativeConstruct()
{
	Super::NativeConstruct();
}

