// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/PlayerController.h"
#include "PC_GreatSword.generated.h"

/**
 * 
 */
UCLASS()
class GREATSWORD_API APC_GreatSword : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
