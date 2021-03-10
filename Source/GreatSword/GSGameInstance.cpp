// Fill out your copyright notice in the Description page of Project Settings.


#include "GSGameInstance.h"

UGSGameInstance::UGSGameInstance()
{
	FString PlayerATKRateDataPath = TEXT("/Game/GameData/DT_PlayerATKRate.DT_PlayerATKRate");
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_PlayerATKRate(*PlayerATKRateDataPath);

	GSCHECK(DT_PlayerATKRate.Succeeded());
	PlayerATKRateTable = DT_PlayerATKRate.Object;
	GSCHECK(PlayerATKRateTable->GetRowMap().Num() > 0);
}

void UGSGameInstance::Init()
{
	Super::Init();
	//GSLOG(Warning, TEXT("AttackMontage - 03 Section_1 ATKRate : %f"),GetPlayerATKRateTable(3)->Section_1);
}

float UGSGameInstance::GetPlayerATKRateTable(int32 AnimMontageIndex, int32 SectionIndex) const
{
	auto Row =  PlayerATKRateTable->FindRow<FPlayerATKRate>(*FString::FromInt(AnimMontageIndex), TEXT(""));

	switch (SectionIndex)
	{
	case 0 :
		return Row->Section_1;

	case 1 : 
		return Row->Section_2;

	case 2 : 
		return Row->Section_3;

	default:
		GSLOG(Error, TEXT("Set Damage Rate Error"));
		break;
	}
	return 0.f;
}