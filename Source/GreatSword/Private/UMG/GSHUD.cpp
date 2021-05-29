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

	if (StatBarWidgetClass)
	{
		StatBarWidget = CreateWidget<UWidget_StatBar>(GetWorld(),StatBarWidgetClass);

		if (StatBarWidget)
		{
			StatBarWidget->AddToViewport();
		}
	}
}

UWidget_StatBar* AGSHUD::GetWidget_StatBar()
{
	return StatBarWidget;
}
