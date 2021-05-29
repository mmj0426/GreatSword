// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGameMode.h"
#include "Player_Controller.h"

#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GSHUD.h"


AGSGameMode::AGSGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter>
	PlayerCharacter(TEXT("/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C"));

	DefaultPawnClass = PlayerCharacter.Class;
	PlayerControllerClass = APlayer_Controller::StaticClass();


	static ConstructorHelpers::FClassFinder<AGSHUD>
	HudClass(TEXT("/Game/Blueprints/UMG/GSHUD.GSHUD_C"));

	HUDClass = HudClass.Class;
}