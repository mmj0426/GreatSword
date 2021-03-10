// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStatComponent.h"

// Sets default values for this component's properties
UBossStatComponent::UBossStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	MaxHP = 100.0f;
	// ...
}


// Called when the game starts
void UBossStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBossStatComponent::InitializeComponent()
{
	CurrentHP = MaxHP;
}

void UBossStatComponent::SetHP(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP-NewDamage, 0.0f,MaxHP);

	GSLOG(Warning, TEXT("Boss CurrentHP : %f"), CurrentHP);

	if (CurrentHP <= 0.0f)
	{
		OnBossHPIsZero.Broadcast();
	}
}
