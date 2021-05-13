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
			BossAnim->PlayDeathMontage();
			SetActorEnableCollision(false);
		});
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
	//CurrentHP = MaxHP;
	
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
	GSLOG(Warning, TEXT("%s"), *CurrentAttackType);
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

	GSGameInstance->Priority_Phase2.Sort();

	//for (int i = 0; i < GSGameInstance->Phase2_Attack.Num(); i++)
	//{
	//	GSLOG(Warning, TEXT("%d"),GSGameInstance->Priority_Phase2[i]);
	//}

	switch (CurrentPhase)
	{
	case EBossPhase::Phase1:
		
		GSGameInstance->Priority_Phase1.Sort();
		CurrentAttackType = GSGameInstance->Phase1_Attack[0].AttackName;

		break;

	case EBossPhase::Phase2:

		GSGameInstance->Priority_Phase2.Sort();
		break;

	case EBossPhase::Phase3:

		break;

	default:
		break;
	}
	
	// TODO : 페이즈를 검사하는 BTS 생성 -> 여기서 CurrentPhase 설정해주기
	

	//CurrentAttackType = GSGameInstance->Phase1_Attack[0]->AttackName;

	

	//GSGameInstance->GetAttack(FName("Phase1_Attack01"))->Priority

	// TODO : datatable에서 우선순위 가져와서 페이즈에 맞는 우선순위 계산 -> CurrentAttackType Setting

	// CurrentAttackType = TEXT("~~");
}
