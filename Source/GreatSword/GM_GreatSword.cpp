// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_GreatSword.h"
#include "PC_GreatSword.h"

AGM_GreatSword::AGM_GreatSword()
{
	static ConstructorHelpers::FClassFinder<ACharacter>
		PlayerCharacter(TEXT("/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C"));

	DefaultPawnClass = PlayerCharacter.Class;
	PlayerControllerClass = APC_GreatSword::StaticClass();
}