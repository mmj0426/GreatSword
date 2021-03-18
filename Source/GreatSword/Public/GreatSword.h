// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(GreatSword, Log, All);

#define GS_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define GS_S(Verbosity) UE_LOG(GreatSword, Verbosity, TEXT("%s"), *GS_CALLINFO)

#define GSLOG(Verbosity,Format, ...) UE_LOG(GreatSword, Verbosity, TEXT("%s %s"), *GS_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

//GSCHECK 매크로를 추가 : 런타임에서 문제가 발생할 때 붉은색 에러로그 뿌리고 함수 반환
#define GSCHECK(Expr, ...){if(!(Expr)) {GSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
