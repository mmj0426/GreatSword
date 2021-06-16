// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "GameFramework/HUD.h"
#include "GSHUD.generated.h"

class UWidget_StatBar;

/**
 *
 */
UCLASS()
class GREATSWORD_API AGSHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGSHUD();

	virtual void BeginPlay() override;

public:
	UWidget_StatBar* GetWidget_PlayerStatBar();
	UWidget_StatBar* GetWidget_BossStatBar();
	UWidget_StatBar* GetWidget_GothicStatBar();

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UWidget_StatBar> PlayerStatWidget;


	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UWidget_StatBar> BossStatWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UWidget_StatBar> GothicStatWidget;

private:
	UPROPERTY()
		class UWidget_StatBar* PlayerStatBar;

	UPROPERTY()
		class UWidget_StatBar* BossStatBar;

	UPROPERTY()
	class UWidget_StatBar* GothicStatBar;

};
