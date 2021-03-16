// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGameMode.h"
#include "GSPlayerController.h"

AGSGameMode::AGSGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter>
		PlayerCharacter(TEXT("/Game/Blueprints/Player/BP_PlayerCharacter.BP_PlayerCharacter_C"));

	DefaultPawnClass = PlayerCharacter.Class;
	PlayerControllerClass = AGSPlayerController::StaticClass();
}