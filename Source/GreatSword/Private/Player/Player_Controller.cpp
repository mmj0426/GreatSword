// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Controller.h"
#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	GSPlayer = Cast<APlayerCharacter>(GetPawn());
}

// Called to bind functionality to input
void APlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &APlayer_Controller::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APlayer_Controller::MoveRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &APlayer_Controller::LookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &APlayer_Controller::Turn);

	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &APlayer_Controller::Walk);
	InputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &APlayer_Controller::Run);

	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayer_Controller::Attack);
	InputComponent->BindAction(TEXT("Smash"), EInputEvent::IE_Pressed, this, &APlayer_Controller::Smash);
	InputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, this, &APlayer_Controller::Evade);
}


void APlayer_Controller::MoveForward(float NewAxisValue)
{
	MoveValue.X = NewAxisValue;
	

	if (GSPlayer && NewAxisValue != 0.0f && GSPlayer->CanMove())
	{
		//GSPlayer->SetPlayerState(EPlayerState::Moving);
		GSPlayer->AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
	}
}

void APlayer_Controller::MoveRight(float NewAxisValue)
{
	MoveValue.Y = NewAxisValue;

	if (GSPlayer && NewAxisValue != 0.0f && GSPlayer->CanMove())
	{
		//GSPlayer->SetPlayerState(EPlayerState::Moving);
		GSPlayer->AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
	}
}

void APlayer_Controller::LookUp(float NewAxisValue)
{
	if (Player && NewAxisValue != 0.0f)
	{
		GSPlayer->AddControllerPitchInput(NewAxisValue);
	}
}

void APlayer_Controller::Turn(float NewAxisValue)
{
	if (Player && NewAxisValue != 0.0f)
	{
		GSPlayer->AddControllerYawInput(NewAxisValue);
	}
}

void APlayer_Controller::Walk()
{
	GSPlayer->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APlayer_Controller::Run()
{
	GSPlayer->GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

void APlayer_Controller::Attack()
{
	GSPlayer->Attack();
}

void APlayer_Controller::Smash()
{
	GSPlayer->Smash();
}

void APlayer_Controller::Evade()
{
	GSPlayer->Evade();
}