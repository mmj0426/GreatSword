// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStatComponent.h"
#include "Boss.h"
#include "GSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_StatBar.h"


// Sets default values for this component's properties
UBossStatComponent::UBossStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	MaxHP = 1000.0f;
	BaseDamage = 100.0f;
	// ...
}

void UBossStatComponent::PostInitProperties()
{
	Super::PostInitProperties();

	CurrentHP = MaxHP;
}

#if WITH_EDITOR
void UBossStatComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CurrentHP = MaxHP;
}
#endif

// Called when the game starts
void UBossStatComponent::BeginPlay()
{
	Super::BeginPlay();	// ...

	//GSLOG(Warning, TEXT("PlayerStat : Beginplay"));
	GetWorld()->GetTimerManager().SetTimer(HUDInitHandle, [&]()
		{
			auto Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			auto Widget = Cast<AGSHUD>(Controller->GetHUD())->GetWidget_StatBar();

			Widget->SetBossHPPercent(FMath::Clamp(MaxHP / MaxHP, 0.f, 1.f));

			GetWorld()->GetTimerManager().ClearTimer(HUDInitHandle);
		}, 0.1f, false);	
}


// Called every frame
void UBossStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	
	// ...
}

void UBossStatComponent::InitializeComponent()
{
	
}

void UBossStatComponent::SetHP(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f,MaxHP);

	auto Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto Widget = Cast<AGSHUD>(Controller->GetHUD())->GetWidget_StatBar();

	Widget->SetBossHPPercent(FMath::Clamp(CurrentHP/MaxHP,0.f,1.f));

	GSLOG(Warning, TEXT("Boss CurrentHP : %f"), CurrentHP);

	if (CurrentHP <= 0.0f)
	{
		OnBossHPIsZero.Broadcast();
	}
}