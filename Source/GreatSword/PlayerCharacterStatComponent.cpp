// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterStatComponent.h"
#include "Lightmass/LightmassImportanceVolume.h"

// Sets default values for this component's properties
UPlayerCharacterStatComponent::UPlayerCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// InitializeComponent를 호출하기 위함
	bWantsInitializeComponent = true;


	MaxHP = 100.0f;
	MaxStamina = 100.0f;
	Damage = 100.0f;

	// ...
}


void UPlayerCharacterStatComponent::PostInitProperties()
{
	Super::PostInitProperties();
	
	CurrentHP = MaxHP;
	CurrentStamina = MaxStamina;
}

#if WITH_EDITOR
void UPlayerCharacterStatComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CurrentHP = MaxHP;
	CurrentStamina = MaxStamina;
}
#endif

// Called when the game starts
void UPlayerCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerCharacterStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO : HP & Stamina 초당 회복
}

void UPlayerCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}
