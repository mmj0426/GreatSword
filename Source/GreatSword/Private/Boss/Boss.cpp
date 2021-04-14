// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Boss_AnimInstance.h"
#include "BossStatComponent.h"
#include "Boss_AIController.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 자연스러운 움직임을 위함
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,480.0f,0.0f);

	// Capsule Component
	GetCapsuleComponent()->SetCapsuleHalfHeight(140.0f);
	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Boss");

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-130.0f),FRotator(0.0f,-90.0f,0.0f));

	MaxHP = 100.0f;

	// Animation
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		Boss_Anim(TEXT("/Game/Blueprints/Enemies/Animation/AnimBP_Boss.AnimBP_Boss_C"));

	if (Boss_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Boss_Anim.Class);
	}

	// Stat
	BossStat = CreateDefaultSubobject<UBossStatComponent>(TEXT("BossStat"));

	AIControllerClass = ABoss_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BossAnim = Cast<UBoss_AnimInstance>(GetMesh()->GetAnimInstance());
	GSCHECK(BossAnim != nullptr);

	BossAnim->OnMontageEnded.AddDynamic(this, &ABoss::MontageEnded);

	BossStat->OnBossHPIsZero.AddLambda([this]()->void
		{
			BossAnim->PlayDeathMontage();
			SetActorEnableCollision(false);
		});
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;
	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ABoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	BossStat->SetHP(Damage);

	return Damage;
}

void ABoss::Attack()
{
	BossAnim->PlayAttackMontage();
}

void ABoss::MontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	GSLOG(Warning, TEXT("Boss Montage Ended"));
	OnAttackEnd.Broadcast();
}
