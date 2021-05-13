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

USTRUCT(BlueprintType)
struct FBossAttack : public FTableRowBase
{
	GENERATED_BODY()
public : 
	FBossAttack() : Phase(0), AttackName(TEXT("")), Priority(-1), ATKRate(0.0f), Cooldown(0.0f) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Phase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AttackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ATKRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Cooldown;

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
	//float GetBossAttackTable(int32 Phase) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TArray<FBossAttack> Phase1_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TArray<int32> Priority_Phase1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TArray<FBossAttack> Phase2_Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TArray<int32> Priority_Phase2;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	//TArray<FBossAttack*> Phase3_Attack;
	

private : 

	UPROPERTY()
	class UDataTable* PlayerATKRateTable;

	UPROPERTY()
	class UDataTable* BossAttackTable;
};
