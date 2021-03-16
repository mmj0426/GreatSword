// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GreatSword.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "GSGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPlayerATKRate : public FTableRowBase
{
	GENERATED_BODY()

public : 
	FPlayerATKRate() : AttackMontageIndex(0),Section_0(0.0f), Section_1(0.0f), Section_2(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 AttackMontageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Section_2;
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

	float GetPlayerATKRateTable(int32 AnimMontageIndex, int32 SectionIndex) const;

private : 
	UPROPERTY()
	class UDataTable* PlayerATKRateTable;

};
