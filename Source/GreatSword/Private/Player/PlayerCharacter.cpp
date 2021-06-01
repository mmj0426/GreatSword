// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Player_Controller.h"
#include "Player_AnimInstance.h"
#include "PlayerCharacterStatComponent.h"
#include "GSGameInstance.h"
#include "Boss.h"
#include "GSHUD.h"
//#include "DrawDebugHelpers.h"

#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(85.0f);
	GetCapsuleComponent()->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));

	//SpringArm

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);

	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 60.0f), FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 600.0f;

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	//Camera

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//Skeletal Mesh

	// /Game/GreatSword/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_Player(TEXT("/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithoutHelmet.SK_MercenaryWarrior_WithoutHelmet"));

	if (SK_Player.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Player.Object);
	}

	//Animation

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		Anim_Player(TEXT("/Game/Blueprints/Player/Animation/AnimBP_Player.AnimBP_Player_C"));

	if (Anim_Player.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Player.Class);
	}


	//Weapon
	FName WeaponSocket(TEXT("Weapon"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);
	WeaponMesh->SetRelativeScale3D(FVector(2.f,2.f,2.f));
	WeaponMesh->SetCollisionProfileName("Nocollision");

	// /Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_Weapon(TEXT("/Game/Blueprints/Player/SM_Great_Sword_5.SM_Great_Sword_5"));

	if (SM_Weapon.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SM_Weapon.Object);
	}

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(WeaponMesh);

	// Attack

	MaxCombo = 4;
	AttackEndComboState();

	// Draw Debug
	//AttackRange = 200.0f;
	//AttackRadius = 50.0f;

	// Character Stat
	CharacterStat = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));

	bCanHP_Recovery = false;
	bCanStamina_Recovery = false;

	
}

void APlayerCharacter::AttackStartComboState()
{
	CurrentMouseInput = EReadyToAttack::None;

	// 콤보 값이 0 ~ MaxCombo-1 사이인지 검사
	GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 0, MaxCombo - 1));
	AttackComboIndex = FMath::Clamp<int32>(AttackComboIndex + 1, 1, MaxCombo);
}

void APlayerCharacter::AttackEndComboState()
{
	CurrentMouseInput = EReadyToAttack::None;

	AttackComboIndex = 0;
	SmashIndex = 0;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	PlayerAnim = Cast<UPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	PlayerAnim->SetCurrentPlayerState(EPlayerState::Idle);
	GSCHECK(nullptr != PlayerAnim);

	//TODO : PlayerAnimMontage AttackHitCheck 노티파이 지우기
	PlayerAnim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::MontageEnded);
	//PlayerAnim->OnAttackHitCheck.AddUObject(this, &APlayerCharacter::AttackCheck);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	//WeaponCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);


	// Combo Attack Delegate
	PlayerAnim->OnNextAttackCheck.AddLambda([this]()->void
		{
			if (CurrentMouseInput == EReadyToAttack::Attack && CanAttack())
			{
				AttackStartComboState();
				SmashIndex = 0;

				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

				GSLOG(Warning, TEXT("On Next Attack Check"));
				PlayerAnim->PlayAttackMontage(AttackComboIndex);
				UseStamina(true);
			}
		});

	PlayerAnim->OnSmashCheck.AddLambda([this]()->void
		{
			//GSLOG(Error, TEXT("OnSmashCheck Lambda"));
			if (CurrentMouseInput == EReadyToAttack::Smash && CanAttack())
			{
				GSCHECK(FMath::IsWithinInclusive<int32>(SmashIndex, 0, PlayerAnim->GetMaxSection() - 1));
				SmashIndex = FMath::Clamp<int32>(SmashIndex + 1, 1, PlayerAnim->GetMaxSection());

				//GSLOG(Error, TEXT("Smash Index : %i"), SmashIndex);

				CurrentMouseInput = EReadyToAttack::None;

				PlayerAnim->SetCurrentCombo(PlayerAnim->GetCurrentCombo() + 1);

				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
				PlayerAnim->JumpToSmashMontageSection(SmashIndex);
				UseStamina(true);

				//PlayerAnim->SetCurrentPlayerState(EPlayerState::Attacking);
			}
			else
			{
				PlayerAnim->SetCurrentPlayerState(EPlayerState::Idle);
				AttackEndComboState();
			}
		});

	CharacterStat->OnPlayerHPIsZero.AddLambda([this]()->void
		{
			PlayerAnim->SetIsDead();
			SetActorEnableCollision(false);
			//UnPossessed();
		});
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	// 플레이어가 달리는지 검사
	if (GetCharacterMovement()->MaxWalkSpeed >= 450.f)
	{
		bCanStamina_Recovery = false;

		if (GetWorldTimerManager().IsTimerActive(Stamina_RecoveryCheckHandle))
		{
			GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
		}

		float waitTime = 3.f;

		GetWorld()->GetTimerManager().SetTimer(Stamina_RecoveryCheckHandle, [&]()
			{
				bCanStamina_Recovery = true;
				GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
			}, waitTime, false); 
	}

	//GSLOG(Warning, TEXT("Current Pawn Speed : %f"), GetCharacterMovement()->MaxWalkSpeed);
	//GSLOG(Warning, TEXT("AttackIndex : %d"),AttackComboIndex);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 패링이 아닐 경우
	if (!isParrying && !isPerfectParrying)
	{
		CharacterStat->SetCurrentHP(FMath::Clamp<float>(CharacterStat->GetCurrentHP() - Damage, 0.0f, CharacterStat->GetMaxHP()));

	}
	if (isPerfectParrying)
	{
		UGameplayStatics::SetGlobalTimeDilation(this, 0.4);
		// 패링 소모값만큼 스태미나 회복
		CharacterStat->SetCurrentStamina(FMath::Clamp<float>(CharacterStat->GetCurrentStamina() + 10.f, 0.f, CharacterStat->GetMaxStamina()));
	}

	bCanHP_Recovery = false;

	// 5초간 피격 여부 검사
	if (GetWorldTimerManager().IsTimerActive(HP_RecoveryCheckHandle))
	{
		GetWorldTimerManager().ClearTimer(HP_RecoveryCheckHandle);
	}

	float waitTime = 5.f;

	GetWorld()->GetTimerManager().SetTimer(HP_RecoveryCheckHandle, [&]()
		{
			bCanHP_Recovery = true;
			GetWorldTimerManager().ClearTimer(HP_RecoveryCheckHandle);
		}, waitTime, false);

	return Damage;
}

void APlayerCharacter::Attack()
{
	// 3초간 Attack 여부 검사
	bCanStamina_Recovery = false;

	if (GetWorldTimerManager().IsTimerActive(Stamina_RecoveryCheckHandle))
	{
		GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
	}

	float waitTime = 3.f;

	GetWorld()->GetTimerManager().SetTimer(Stamina_RecoveryCheckHandle, [&]()
		{
			bCanStamina_Recovery = true;
			GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
		}, waitTime, false);



	if (PlayerAnim->GetCurrentPlayerState() == EPlayerState::Attacking)
	{
		//GSLOG(Warning, TEXT(" Current  Player State : Attack"))
		GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 1, MaxCombo));
		CurrentMouseInput = EReadyToAttack::Attack;
	}
	else
	{
		//GSCHECK(AttackComboIndex == 0);
		AttackStartComboState();
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		PlayerAnim->PlayAttackMontage(AttackComboIndex);
		UseStamina(true);

		PlayerAnim->SetCurrentPlayerState(EPlayerState::Attacking);
	}

}

void APlayerCharacter::Smash()
{
	// 3초간 Smash 여부 검사
	bCanStamina_Recovery = false;

	if (GetWorldTimerManager().IsTimerActive(Stamina_RecoveryCheckHandle))
	{
		GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
	}

	float waitTime = 3.f;

	GetWorld()->GetTimerManager().SetTimer(Stamina_RecoveryCheckHandle, [&]()
		{
			bCanStamina_Recovery = true;
			GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
		}, waitTime, false);


	if (PlayerAnim->GetCurrentPlayerState() == EPlayerState::Attacking)
	{
		CurrentMouseInput = EReadyToAttack::Smash;
	}
}

void APlayerCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	GSLOG(Error, TEXT("Player Montage Ended"));

	if (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Attacking)
	{
		PlayerAnim->SetCurrentPlayerState(EPlayerState::Idle);
	}

	if (PlayerAnim->GetCurrentCombo() >= PlayerAnim->GetMaxSection())
	{
		PlayerAnim->SetCurrentPlayerState(EPlayerState::Idle);
		//GSLOG(Error, TEXT("CurrentCombo : %i , MaxSection : %i"),PlayerAnim->GetCurrentCombo(), PlayerAnim->GetMaxSection());
		AttackEndComboState();
	}
}

void APlayerCharacter::Evade()
{
	// 3초간 회피 여부 검사	
	bCanStamina_Recovery = false;

	if (GetWorldTimerManager().IsTimerActive(Stamina_RecoveryCheckHandle))
	{
		GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
	}

	float waitTime = 3.f;

	GetWorld()->GetTimerManager().SetTimer(Stamina_RecoveryCheckHandle, [&]()
		{
			bCanStamina_Recovery = true;
			GetWorldTimerManager().ClearTimer(Stamina_RecoveryCheckHandle);
		}, waitTime, false);


	auto GSController = Cast<APlayer_Controller>(GetController());

	if (GSController->GetMoveValue().IsZero() && CanEvade())
	{
		Parrying();
	}
	else if (!GSController->GetMoveValue().IsZero() && CanEvade())
	{
		Dodge();
	}
}

void APlayerCharacter::Parrying()
{
	PlayerAnim->PlayParryingMontage();
	UseStamina(false);
}

void APlayerCharacter::Dodge()
{
	SetPlayerRotation();

	PlayerAnim->PlayDodgeMontage();
	UseStamina(false);
}

void APlayerCharacter::SetPlayerRotation()
{
	auto GSController = Cast<APlayer_Controller>(GetController());

	int32 RotationRate = 0;
	int32 MoveValueX = static_cast<int32>(GSController->GetMoveValue().X);
	int32 MoveValueY = static_cast<int32>(GSController->GetMoveValue().Y);

	if (MoveValueX != 0 && MoveValueY == 0)
	{
		RotationRate = (MoveValueX) < 0 ? 180 : 0;
	}
	else
	{
		RotationRate = MoveValueY * (90 + -(MoveValueX * 45));
	}

	SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw + RotationRate, 0.0f));

}

bool APlayerCharacter::CanEvade() const
{
	return (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Attacking)
		&& (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Parrying)
		&& (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Dodge);
}

bool APlayerCharacter::CanMove() const
{
	return (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Attacking)
		&& (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Parrying)
		&& (PlayerAnim->GetCurrentPlayerState() != EPlayerState::Dodge);
}

bool APlayerCharacter::CanAttack() const
{
	//auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//float UseStamina = GSGameInstance->GetPlayerStaminaTable(AttackComboIndex, SmashIndex);

	//if (CharacterStat->GetCurrentStamina() >= UseStamina)
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
	return true;
}

void APlayerCharacter::UseStamina(bool isAttack)
{
	if (isAttack)
	{
		auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		float UseStamina = GSGameInstance->GetPlayerStaminaTable(PlayerAnim->GetCurrentAttackIndex(), PlayerAnim->GetCurrentSectionIndex());

		CharacterStat->SetCurrentStamina(FMath::Clamp<float>(CharacterStat->GetCurrentStamina() - UseStamina, 0.f, CharacterStat->GetMaxStamina()));
	}
	else
	{
		// Evade Stamina
		CharacterStat->SetCurrentStamina(FMath::Clamp<float>(CharacterStat->GetCurrentStamina() - 10.f, 0.f, CharacterStat->GetMaxStamina()));
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GSLOG(Warning,TEXT("OnOverlapBegin"));

	auto HitResult = Cast<ABoss>(OtherActor);
	auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (HitResult != nullptr && PlayerAnim->GetCurrentPlayerState() == EPlayerState::Attacking)
	{
		// Player Stat에서 현재 진행중인 애니메이션 몽타주 인덱스와 섹션 인덱스를 통해 데미지 적용율을 가지고와 데미지를 계산함
		float HitDamage = CharacterStat->GetDamage() * (GSGameInstance->GetPlayerATKRateTable(PlayerAnim->GetCurrentAttackIndex(), PlayerAnim->GetCurrentSectionIndex())) / 100.0f;

		//GSLOG(Warning, TEXT("Hit Damage : %f "), HitDamage);

		//데미지 적용
		UGameplayStatics::ApplyDamage(HitResult, HitDamage, NULL, GetController(), NULL);
	}
}

#pragma region OverlapEnd
//void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}
#pragma endregion

#pragma region TraceChannel AttackSystem AttackHitCheck Delegate
//void APlayerCharacter::AttackCheck()
//{
//	// 충돌된 액터에 관련된 정보를 얻기 위한 구조체
//	FHitResult HitResult;
//	// 플레이어 자신은 탐색되지 않도록 하기위한 (무시할 액터 목록을 담은)구조체
//	FCollisionQueryParams Params(NAME_None, false, this);
//
//	bool bResult = GetWorld()->SweepSingleByChannel(
//	HitResult,
//	GetActorLocation(),
//	GetActorLocation() + GetActorForwardVector() * 200.0f,
//	FQuat::Identity,
//	ECollisionChannel::ECC_GameTraceChannel1,
//	FCollisionShape::MakeSphere(50.0f),
//	Params);
//
//	// Draw Debug
//	#if ENABLE_DRAW_DEBUG
//
//	FVector TraceVector = GetActorForwardVector() * AttackRange;
//	FVector Center = GetActorLocation() + TraceVector * 0.5;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
//	FColor DrawColor = bResult?FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule(
//		GetWorld(),
//		Center,
//		HalfHeight,
//		AttackRadius,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime
//	);
//
//	#endif
//
//	auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	
//	if (bResult && HitResult.Actor.IsValid())
//	{
//		GSLOG(Warning, TEXT("Hit Actor Name : %s"),*HitResult.Actor->GetName());
//
//		// Player Stat에서 현재 진행중인 애니메이션 몽타주 인덱스와 섹션 인덱스를 통해 데미지 적용율을 가지고와 데미지를 계산함
//		float HitDamage =	CharacterStat->GetDamage() * (GSGameInstance->GetPlayerATKRateTable(PlayerAnim->GetCurrentAttackIndex(), PlayerAnim->GetCurrentSectionIndex()))/100.0f;
//
//		GSLOG(Warning, TEXT("Hit Damage : %f"),HitDamage);
//		//데미지 적용
//		UGameplayStatics::ApplyDamage(HitResult.GetActor(), HitDamage, NULL, GetController(), NULL);
//	}
//
//}
#pragma endregion
