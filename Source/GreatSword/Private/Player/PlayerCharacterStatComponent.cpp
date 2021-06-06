// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterStatComponent.h"
#include "Lightmass/LightmassImportanceVolume.h"
#include "PlayerCharacter.h"
#include "Widget_StatBar.h"
#include "GSHUD.h"

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


	//GSLOG(Warning, TEXT("PlayerStat : Beginplay"));

	GetWorld()->GetTimerManager().SetTimer(HP_RecoveryHandle, this, &UPlayerCharacterStatComponent::HP_Recovery, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(Stamina_RecoveryHandle, this, &UPlayerCharacterStatComponent::Stamina_Recovery, 1.0f, true);

	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto Controller = Cast<APlayerController>(Player->GetController());
	auto Widget = Cast<AGSHUD>(Controller->GetHUD())->GetWidget_PlayerStatBar();

	Widget->SetPlayerHPPercent(FMath::Clamp<float>(MaxHP / MaxHP, 0.f, 1.f));
	Widget->SetPlayerStaminaPercent(FMath::Clamp<float>(MaxStamina / MaxStamina, 0.f, 1.f));
	// ...
	
}

void UPlayerCharacterStatComponent::SetCurrentHP(float HP)
{
	CurrentHP = HP;

	GSLOG(Warning, TEXT("Player CurrentHP : %f"), CurrentHP);

	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto Controller = Cast<APlayerController>(Player->GetController());
	auto Widget = Cast<AGSHUD>(Controller->GetHUD())->GetWidget_PlayerStatBar();

	Widget->SetPlayerHPPercent(FMath::Clamp<float>(CurrentHP / MaxHP, 0.f, 1.f));

	if (CurrentHP <= 0.f)
	{
		OnPlayerHPIsZero.Broadcast();
	}
}

void UPlayerCharacterStatComponent::SetCurrentStamina(float Stamina)
{
	CurrentStamina = Stamina;

	auto Player = Cast<APlayerCharacter>(GetOwner());
	auto Controller = Cast<APlayerController>(Player->GetController());
	auto Widget = Cast<AGSHUD>(Controller->GetHUD())->GetWidget_PlayerStatBar();

	Widget->SetPlayerStaminaPercent(FMath::Clamp<float>(CurrentStamina / MaxStamina, 0.f, 1.f));

	//GSLOG(Warning, TEXT("Player Stamina : %f"), CurrentStamina);
}

void UPlayerCharacterStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerCharacterStatComponent::HP_Recovery()
{
	// 초당 HP 회복
	auto Player = Cast<APlayerCharacter>(GetOwner());

	if (nullptr != Player && Player->GetCanHP_Recovery())
	{
		CurrentHP = FMath::Clamp<float>(CurrentHP + 1.f, 0.f, MaxHP);

		SetCurrentHP(CurrentHP);
	}
}

void UPlayerCharacterStatComponent::Stamina_Recovery()
{
	auto Player = Cast<APlayerCharacter>(GetOwner());

	// 초당 Stamina 회복
	if (nullptr != Player && Player->GetCanStamina_Recovery())
	{
		CurrentStamina = FMath::Clamp<float>(CurrentStamina + 15.f, 0.f, MaxStamina);

		SetCurrentStamina(CurrentStamina);
	}
}

void UPlayerCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}
