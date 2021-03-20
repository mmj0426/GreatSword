// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Player_AnimInstance.h"
#include "PlayerCharacterStatComponent.h"
#include "GSGameInstance.h"
//#include "DrawDebugHelpers.h"

#include "UObject/ConstructorHelpers.h"
#include "UObject/Class.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Boss.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement  = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(85.0f);
	GetCapsuleComponent()->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));
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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	SK_Player(TEXT("/Game/GreatSword/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (SK_Player.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Player.Object);
	}

	//Animation

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
	Anim_Player(TEXT("/Game/Blueprints/Player/Animation/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter_C"));
	
	if (Anim_Player.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Player.Class);
	}


	//Weapon
	FName WeaponSocket(TEXT("Weapon"));

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(),WeaponSocket);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_Weapon(TEXT("/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02"));

	if (SM_Weapon.Succeeded())
	{
		Weapon->SetStaticMesh(SM_Weapon.Object);
	}
	
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(Weapon);
	WeaponCollision->SetCollisionProfileName("Player_Weapon");

	// Attack
	CurrentState = EPlayerState::Idle;

	MaxCombo = 4;
	AttackEndComboState();

	// Draw Debug
	//AttackRange = 200.0f;
	//AttackRadius = 50.0f;

	// Character Stat
	CharacterStat = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));
}

void APlayerCharacter::AttackStartComboState()
{
	CurrentMouseInput = EMouseControll::None;

	// �޺� ���� 0 ~ MaxCombo-1 �������� �˻�
	GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 0,MaxCombo -1));
	AttackComboIndex = FMath::Clamp<int32>(AttackComboIndex+1, 1, MaxCombo);
}

void APlayerCharacter::AttackEndComboState()
{
	CurrentMouseInput = EMouseControll::None;

	AttackComboIndex = 0;
	SmashIndex = 0;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerAnim = Cast<UPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	GSCHECK(nullptr != PlayerAnim);

	//TODO : PlayerAnimMontage AttackHitCheck ��Ƽ���� �����
	PlayerAnim->OnMontageEnded.AddDynamic(this,&APlayerCharacter::MontageEnded);
	//PlayerAnim->OnAttackHitCheck.AddUObject(this, &APlayerCharacter::AttackCheck);

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	//WeaponCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	 
	// Combo Attack Delegate
	PlayerAnim->OnNextAttackCheck.AddLambda([this]()->void
		{
			if (CurrentMouseInput == EMouseControll::Left)
			{
				AttackStartComboState();
				SmashIndex = 0;
				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
				PlayerAnim->PlayAttackMontage(AttackComboIndex);
			}
		});

	PlayerAnim->OnSmashCheck.AddLambda([this]()->void
		{
			//GSLOG(Error, TEXT("OnSmashCheck Lambda"));
			if (CurrentMouseInput == EMouseControll::Right)
			{
				GSCHECK(FMath::IsWithinInclusive<int32>(SmashIndex, 0, PlayerAnim->GetMaxSection()-1));
				SmashIndex = FMath::Clamp<int32>(SmashIndex+1, 1, PlayerAnim->GetMaxSection());
				
				//GSLOG(Error, TEXT("Smash Index : %i"), SmashIndex);

				CurrentMouseInput = EMouseControll::None;
				///bRMDown = false;
				CurrentState = EPlayerState::Attacking;
				PlayerAnim->SetCurrentCombo(PlayerAnim->GetCurrentCombo() + 1);
				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
				PlayerAnim->JumpToSmashMontageSection(SmashIndex);
			}
			else
			{
				CurrentState = EPlayerState::Idle;
				AttackEndComboState();
			}
		});
}
// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Released, this, &APlayerCharacter::Walk);
	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Pressed, this, &APlayerCharacter::Run);

	PlayerInputComponent->BindAction(TEXT("Attack"),EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Smash"),EInputEvent::IE_Pressed, this, &APlayerCharacter::Smash);
	PlayerInputComponent->BindAction(TEXT("Evade"),EInputEvent::IE_Pressed, this, &APlayerCharacter::Evade);
}

void APlayerCharacter::MoveForward(float NewAxisValue)
{
	MoveValue.X = NewAxisValue;
	if (Controller != nullptr && NewAxisValue != 0.0f && (CurrentState != EPlayerState::Attacking))
	{
		CurrentState = EPlayerState::Moving;
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
	}
}

void APlayerCharacter::MoveRight(float NewAxisValue)
{	
	MoveValue.Y = NewAxisValue;
	if (Controller != nullptr && NewAxisValue != 0.0f && (CurrentState != EPlayerState::Attacking))
	{
		CurrentState = EPlayerState::Moving;
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
	}
}

void APlayerCharacter::LookUp(float NewAxisValue)
{
	if (Controller != nullptr && NewAxisValue != 0.0f)
	{
		AddControllerPitchInput(NewAxisValue);
	}
}

void APlayerCharacter::Turn(float NewAxisValue)
{
	if (Controller != nullptr && NewAxisValue != 0.0f)
	{
		AddControllerYawInput(NewAxisValue);
	}
}

void APlayerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APlayerCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

void APlayerCharacter::Attack()
{
	if (CurrentState == EPlayerState::Attacking)
	{
		GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 1, MaxCombo));
		CurrentMouseInput = EMouseControll::Left;
	}
	else
	{
		GSCHECK(AttackComboIndex == 0);
		AttackStartComboState();
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		PlayerAnim->PlayAttackMontage(AttackComboIndex);
		CurrentState = EPlayerState::Attacking;
	}
}

void APlayerCharacter::Smash()
{
	if(CurrentState == EPlayerState::Attacking)
	{
		CurrentMouseInput = EMouseControll::Right;
	}
}

void APlayerCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	if (CurrentState != EPlayerState::Attacking)
	{
		CurrentState = EPlayerState::Idle;
	}

	if (PlayerAnim->GetCurrentCombo() >= PlayerAnim->GetMaxSection())
	{
		CurrentState = EPlayerState::Idle;
		//GSLOG(Error, TEXT("CurrentCombo : %i , MaxSection : %i"),PlayerAnim->GetCurrentCombo(), PlayerAnim->GetMaxSection());
		AttackEndComboState();
	}
}

void APlayerCharacter::Evade()
{ 
	if (MoveValue.IsZero() && CanEvade())
	{
		Parrying();
	}
	else if (!MoveValue.IsZero() && CanEvade())
	{
		Dodge();
	}
}

void APlayerCharacter::Parrying()
{
	CurrentState = EPlayerState::Parrying;
	PlayerAnim->PlayParryingMontage();
}

void APlayerCharacter::Dodge()
{
	CurrentState = EPlayerState::Dodge;

	SetPlayerRotation();

	PlayerAnim->PlayDodgeMontage();
}

void APlayerCharacter::SetPlayerRotation()
{	
	int32 RotationRate = 0;
	int32 MoveValueX = static_cast<int32>(MoveValue.X);
	int32 MoveValueY = static_cast<int32>(MoveValue.Y);

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

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto HitResult = Cast<ABoss>(OtherActor);
	auto GSGameInstance = Cast<UGSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (HitResult != nullptr && CurrentState == EPlayerState::Attacking)
	{
		// Player Stat���� ���� �������� �ִϸ��̼� ��Ÿ�� �ε����� ���� �ε����� ���� ������ �������� ������� �������� �����
		float HitDamage = CharacterStat->GetDamage() * (GSGameInstance->GetPlayerATKRateTable(PlayerAnim->GetCurrentAttackIndex(), PlayerAnim->GetCurrentSectionIndex())) / 100.0f;

		GSLOG(Warning, TEXT("Hit Damage : %f"), HitDamage);
		//������ ����
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
//	// �浹�� ���Ϳ� ���õ� ������ ��� ���� ����ü
//	FHitResult HitResult;
//	// �÷��̾� �ڽ��� Ž������ �ʵ��� �ϱ����� (������ ���� ����� ����)����ü
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
//		// Player Stat���� ���� �������� �ִϸ��̼� ��Ÿ�� �ε����� ���� �ε����� ���� ������ �������� ������� �������� �����
//		float HitDamage =	CharacterStat->GetDamage() * (GSGameInstance->GetPlayerATKRateTable(PlayerAnim->GetCurrentAttackIndex(), PlayerAnim->GetCurrentSectionIndex()))/100.0f;
//
//		GSLOG(Warning, TEXT("Hit Damage : %f"),HitDamage);
//		//������ ����
//		UGameplayStatics::ApplyDamage(HitResult.GetActor(), HitDamage, NULL, GetController(), NULL);
//	}
//
//}
#pragma endregion
