// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(GreatSword, Log, All);

#define GS_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define GS_S(Verbosity) UE_LOG(GreatSword, Verbosity, TEXT("%s"), *GS_CALLINFO)

#define GSLOG(Verbosity,Format, ...) UE_LOG(GreatSword, Verbosity, TEXT("%s %s"), *GS_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//GSCHECK ��ũ�θ� �߰� : ��Ÿ�ӿ��� ������ �߻��� �� ������ �����α� �Ѹ��� �Լ� ��ȯ
#define GSCHECK(Expr, ...){if(!(Expr)) {GSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
