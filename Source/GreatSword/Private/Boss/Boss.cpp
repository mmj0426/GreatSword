// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Boss_AnimInstance.h"
#include "BossStatComponent.h"
#include "Boss_AIController.h"
#include "GSGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABoss::ABoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 자연스러운 움직임을 위함
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);

	// Capsule Component
	GetCapsuleComponent()->SetCapsuleHalfHeight(140.0f);
	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Boss");

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));

	//MaxHP = 100.0f;
	IsAlive = true;

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

	CurrentPhase = EBossPhase::Phase1;

	judgeAttack = 0;
	IsPhase2FirstEntry = true;
	IsPhase3FirstEntry = true;
}

void ABoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BossAnim = Cast<UBoss_AnimInstance>(GetMesh()->GetAnimInstance());
	GSCHECK(BossAnim != nullptr);

	BossAnim->OnMontageEnded.AddDynamic(this, &ABoss::MontageEnded);

	BossStat->OnBossHPIsZero.AddLambda([this]()->void
		{
			IsAlive = false;
			BossAnim->SetDeadAnim();
			SetActorEnableCollision(false); 
			UGameplayStatics::OpenLevel(this, TEXT("Level03_2"));
		});
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();

}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

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

void ABoss::BossAttack()
{
	DecideAttackType();
	//GSLOG(Warning, TEXT("%d"), HitDamage);
	GSLOG(Warning, TEXT("%s"), *CurrentAttackType);
	//GSLOG(Warning, TEXT("%d"), judgeAttack);
	BossAnim->PlayAttackMontage(CurrentAttackType);
}

void ABoss::MontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	GSLOG(Warning, TEXT("Boss Montage Ended"));
	OnAttackEnd.Broadcast();
}

void ABoss::DecideAttackType()
{
	auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:

		if (2 == judgeAttack)
		{
			// Front01
			CurrentAttackType = GSGameInstance->Phase1_Attack[1].AttackName;
			judgeAttack += GSGameInstance->Phase1_Attack[1].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase1_Attack[1].ATKRate;
			break;
		}

		// Ground01
		CurrentAttackType = GSGameInstance->Phase1_Attack[0].AttackName;
		judgeAttack += GSGameInstance->Phase1_Attack[0].JudgeAttack;
		HitDamage = BossStat->BaseDamage * GSGameInstance->Phase1_Attack[0].ATKRate;
		break;

	case EBossPhase::Phase2:

		if (true == IsPhase2FirstEntry)
		{
			judgeAttack = 0;
		}

		//if (TEXT("Jump") == CurrentAttackType || true == CanDash)
		//{
		//	//Dash
		//	CurrentAttackType = GSGameInstance->Phase2_Attack[5].AttackName;
		//	judgeAttack += GSGameInstance->Phase2_Attack[5].JudgeAttack;
		//	HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[5].ATKRate;
		//	CanDash = false;
		//	break;
		//}
		if (2 == judgeAttack)
		{
			// Grount01-2
			CurrentAttackType = GSGameInstance->Phase2_Attack[1].AttackName;
			judgeAttack += GSGameInstance->Phase2_Attack[1].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[1].ATKRate;
			break;
		}
		if (4 == judgeAttack)
		{
			// L_Foot, R_Foot 랜덤으로 지정
			int32 index = FMath::RandRange(2, 3);
			CurrentAttackType = GSGameInstance->Phase2_Attack[index].AttackName;
			judgeAttack += GSGameInstance->Phase2_Attack[index].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[index].ATKRate;
			break;
		}
		if (true == IsPhase2FirstEntry || 6 == judgeAttack)
		{
			// Jump
			CurrentAttackType = GSGameInstance->Phase2_Attack[4].AttackName;
			judgeAttack += GSGameInstance->Phase2_Attack[4].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[4].ATKRate;
			IsPhase2FirstEntry = false;
			break;
		}


		CurrentAttackType = GSGameInstance->Phase2_Attack[0].AttackName;
		judgeAttack += GSGameInstance->Phase2_Attack[0].JudgeAttack;
		HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[0].ATKRate;

		break;

	case EBossPhase::Phase3:

		if (true == IsPhase3FirstEntry)
		{
			judgeAttack = 0;
		}

		//if (TEXT("Jump") == CurrentAttackType || true == CanDash)
		//{
		//	//Dash
		//	CurrentAttackType = GSGameInstance->Phase2_Attack[5].AttackName;
		//	judgeAttack += GSGameInstance->Phase2_Attack[5].JudgeAttack;
		//	HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[5].ATKRate;
		//	CanDash = false;
		//	break;
		//}
		// Ground02
		if (true == IsPhase3FirstEntry || 1 == judgeAttack)
		{
			CurrentAttackType = GSGameInstance->Phase3_Attack[1].AttackName;
			judgeAttack += GSGameInstance->Phase3_Attack[1].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase3_Attack[1].ATKRate;

			IsPhase3FirstEntry = false;
			break;
		}
		if (4 == judgeAttack)
		{
			// L_Foot, R_Foot 랜덤으로 지정
			int32 index = FMath::RandRange(2, 3);
			CurrentAttackType = GSGameInstance->Phase3_Attack[index].AttackName;
			judgeAttack += GSGameInstance->Phase3_Attack[index].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase3_Attack[index].ATKRate;
			break;
		}
		if (6 == judgeAttack)
		{
			// Jump
			CurrentAttackType = GSGameInstance->Phase2_Attack[4].AttackName;
			judgeAttack += GSGameInstance->Phase2_Attack[4].JudgeAttack;
			HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[4].ATKRate;
			IsPhase2FirstEntry = false;
			break;
		}


		// Front03
		CurrentAttackType = GSGameInstance->Phase3_Attack[0].AttackName;
		judgeAttack += GSGameInstance->Phase3_Attack[0].JudgeAttack;
		HitDamage = BossStat->BaseDamage * GSGameInstance->Phase2_Attack[0].ATKRate;

		break;

	default:

		break;
	}
	
}
