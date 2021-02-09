// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_GreatSword.h"

void APC_GreatSword::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}