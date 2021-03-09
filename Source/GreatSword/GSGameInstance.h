// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "GSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FGSCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public : 
	FGSCharacterData() : AttackMontageNum(0),Section_1(0.0f), Section_2(0.0f), Section_3(0.0f) {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AttackMontageNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_3;
};

/**
 * 
 */
UCLASS()
class GREATSWORD_API UGSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public : 
	UGSGameInstance();
	virtual void Init() override;
	

};
