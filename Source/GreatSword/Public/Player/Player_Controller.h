// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/PlayerController.h"
#include "Player_Controller.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent()override;

private : 
	//Movement
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Run();
	void Walk();

	void Attack();
	void Smash();
	void Evade();

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	FVector MoveValue;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	class APlayerCharacter* GSPlayer;

public : 
	FVector GetMoveValue() const { return MoveValue; }

};
