// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/PlayerController.h"
#include "GSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API AGSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
