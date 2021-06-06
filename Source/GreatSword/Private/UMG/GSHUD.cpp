// Fill out your copyright notice in the Description page of Project Settings.


#include "GSHUD.h"
#include "Widget_Statbar.h"

AGSHUD::AGSHUD()
{

}

void AGSHUD::BeginPlay()
{
	Super::BeginPlay();

	//GSLOG(Warning, TEXT("PlayerStat : Beginplay"));

	if (PlayerStatWidget)
	{
		PlayerStatBar = CreateWidget<UWidget_StatBar>(GetWorld(),PlayerStatWidget);

		if (PlayerStatBar)
		{
			PlayerStatBar->AddToViewport();
		}
	}

	if (BossStatWidget)
	{
		BossStatBar = CreateWidget<UWidget_StatBar>(GetWorld(), BossStatWidget);

		if (BossStatBar)
		{
			BossStatBar->AddToViewport();
		}
	}
}

UWidget_StatBar* AGSHUD::GetWidget_PlayerStatBar()
{
	return PlayerStatBar;
}

UWidget_StatBar* AGSHUD::GetWidget_BossStatBar()
{
	return BossStatBar;
}
