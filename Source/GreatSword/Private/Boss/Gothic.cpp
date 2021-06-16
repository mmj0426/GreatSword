// Fill out your copyright notice in the Description page of Project Settings.


#include "Gothic.h"
#include "Gothic_AnimInstance.h"
#include "GSHUD.h"
#include "Widget_StatBar.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AGothic::AGothic()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 자연스러운 움직임을 위함
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);

	IsAlive = true;

	MaxHP = 500.f;

	IsOddNumber = false;

}

void AGothic::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CurrentHP = MaxHP;

	GothicAnim = Cast<UGothic_AnimInstance>(GetMesh()->GetAnimInstance());
	GothicAnim->SetCurrentState(EGothicState::Idle);
	GSCHECK(GothicAnim != nullptr);

	GothicAnim->OnMontageEnded.AddDynamic(this, &AGothic::MontageEnded);
}

// Called when the game starts or when spawned
void AGothic::BeginPlay()
{
	Super::BeginPlay();

	//GSLOG(Warning, TEXT("Gothic Begin Play"));

	GetWorld()->GetTimerManager().SetTimer(WidgetInitHandle, [&]()
		{
			auto playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			auto Widget = Cast<AGSHUD>(playerController->GetHUD())->GetWidget_GothicStatBar();

			//Widget->SetBossHPPercent(FMath::Clamp(MaxHP / MaxHP, 0.f, 1.f));

			Widget->AddToViewport();
			SetCurrentHP(0.f);

			GetWorld()->GetTimerManager().ClearTimer(WidgetInitHandle);
		}, 0.5f, false);

}

void AGothic::GothicAttack()
{
	int32 index = DecideAttack();

	GothicAnim->PlayAttackMontage(index);

	//GSLOG(Warning, TEXT("Gothic Attack Index : %d"), index);
}

int32 AGothic::DecideAttack()
{
	// 홀수번째 공격일 때
	if (IsOddNumber)
	{
		int32 index = FMath::RandRange(0,1);
		IsOddNumber = false;
		return index;
	}
	// 짝수번째 공격일 때
	else
	{
		int32 index = FMath::RandRange(2,4);
		IsOddNumber = true;
		return index;
	}
}

// Called every frame
void AGothic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AGothic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



float AGothic::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);

	SetCurrentHP(Damage);

	return Damage;

}

void AGothic::MontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	GothicAnim->SetCurrentState(EGothicState::Idle);
	OnAttackEnd.Broadcast();
}

void AGothic::SetCurrentHP(float damage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - damage, 0.f, MaxHP);
	

	auto playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto Widget = Cast<AGSHUD>(playerController->GetHUD())->GetWidget_GothicStatBar();

	Widget->SetGothicHPPercent(FMath::Clamp<float>(CurrentHP/MaxHP,0.f,1.f));

	GSLOG(Warning, TEXT("Gothic CurrentHP : %f"), CurrentHP);

	if (CurrentHP <= 0.0f)
	{
		IsAlive = false;
		SetActorEnableCollision(false);
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, [&]()
			{
				Destroy();
			}, 3.f, false);
		
	}
}

