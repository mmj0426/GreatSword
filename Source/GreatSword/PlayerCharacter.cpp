// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GSAnimInstance.h"
#include "DrawDebugHelpers.h"

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
	SM_Player(TEXT("/Game/GreatSword/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (SM_Player.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM_Player.Object);
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
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>
			SK_Weapon(TEXT("/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02"));

		if (SK_Weapon.Succeeded())
		{
			Weapon->SetStaticMesh(SK_Weapon.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	// Attack

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	// Draw Debug
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	IsMoving = false;

	// Evade

	// !< Legacy
	/*IsParrying = false;*/

}

void APlayerCharacter::AttackStartComboState()
{
	bLMDown = false;
	bRMDown = false;

	// 콤보 값이 0 ~ MaxCombo-1 사이인지 검사
	GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 0,MaxCombo -1));
	AttackComboIndex = FMath::Clamp<int32>(AttackComboIndex+1, 1, MaxCombo);
}

void APlayerCharacter::AttackEndComboState()
{
	bLMDown = false;
	bRMDown = false;

	AttackComboIndex = 0;
	SmashIndex = 0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GSAnim = Cast<UGSAnimInstance>(GetMesh()->GetAnimInstance());
	GSCHECK(nullptr != GSAnim);

	GSAnim->OnMontageEnded.AddDynamic(this,&APlayerCharacter::OnAttackMontageEnded);

	GSAnim->OnAttackHitCheck.AddUObject(this, &APlayerCharacter::AttackCheck);
	// Delegate 함수 등록
	GSAnim->OnNextAttackCheck.AddLambda([this]()->void
		{
			if (bLMDown)
			{
				AttackStartComboState();
				SmashIndex = 0;
				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
				GSAnim->PlayAttackMontage(AttackComboIndex);
			}
			//else
			//{
			//	GSLOG(Warning, TEXT("NextCheck Else"));
			//	IsAttacking = false;
			//	AttackEndComboState();
			//}
		});

	GSAnim->OnSmashCheck.AddLambda([this]()->void
		{
			//GSLOG(Error, TEXT("OnSmashCheck Lambda"));
			if (bRMDown)
			{
				GSCHECK(FMath::IsWithinInclusive<int32>(SmashIndex, 0, GSAnim->GetMaxSection()-1));
				SmashIndex = FMath::Clamp<int32>(SmashIndex+1, 1, GSAnim->GetMaxSection());
				
				//GSLOG(Error, TEXT("Smash Index : %i"), SmashIndex);

				bRMDown = false;
				IsAttacking = true;
				GSAnim->SetCurrentCombo(GSAnim->GetCurrentCombo() + 1);
				SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
				GSAnim->JumpToSmashMontageSection(SmashIndex);
			}
			else
			{
				IsAttacking = false;
				AttackEndComboState();
			}
		});

		

	//! < Legacy
	/*GSAnim->OnSmashCheck.AddLambda([this]()->void
		{

			if (IsSmashInputOn)
			{
				GSLOG(Warning, TEXT("Smash Check"));
				GSAnim->PlaySmashMontage();
				GSAnim->JumpToSmashMontageSection(CurrentCombo);
				IsAttacking = true;
			}
		});*/

	// Parrying End Delegate

	//GSAnim->OnParryingEnd.AddLambda([this]()->void
	//	{
	//		IsParrying = false;
	//	}
	//);

	//! < Dodge End Delegate
	//! 
	//GSAnim->OnDodgeEnd.AddLambda([this]()->void
	//	{
	//		IsDodge = false;
	//	});
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
	if (Controller != nullptr && NewAxisValue != 0.0f && !IsAttacking)
	{
		IsMoving = true;
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
	}
}

void APlayerCharacter::MoveRight(float NewAxisValue)
{	
	MoveValue.Y = NewAxisValue;
	if (Controller != nullptr && NewAxisValue != 0.0f && !IsAttacking)
	{
		IsMoving = true;
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
	if (IsAttacking)
	{
		GSCHECK(FMath::IsWithinInclusive<int32>(AttackComboIndex, 1, MaxCombo));
		bLMDown = true;
	}
	else
	{
		GSCHECK(AttackComboIndex == 0);
		AttackStartComboState();
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
		GSAnim->PlayAttackMontage(AttackComboIndex);
		IsAttacking = true;
	}
}

void APlayerCharacter::Smash()
{
	if(IsAttacking)
	{
		bRMDown = true;
	}
}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupeted)
{
	IsParrying = false;
	IsDodge = false;

	if (GSAnim->GetCurrentCombo() >= GSAnim->GetMaxSection())
	{
		IsAttacking = false;
		//GSLOG(Error, TEXT("CurrentCombo : %i , MaxSection : %i"),GSAnim->GetCurrentCombo(), GSAnim->GetMaxSection());
		AttackEndComboState();
	}
}

void APlayerCharacter::Evade()
{ 
	if (!IsAttacking)
	{
		if (MoveValue.IsZero() && !IsDodge)
		{
			Parrying();
		}
		else if (!MoveValue.IsZero() && !IsParrying && !IsDodge)
		{
			Dodge();
		}
	}
}

void APlayerCharacter::Parrying()
{
	IsParrying = true;
	GSAnim->PlayParryingMontage();
}

void APlayerCharacter::Dodge()
{
	IsDodge = true;

	// 구르기 시 W,A,S,D 키 값에 따라 방향 회전
	if (MoveValue.X == 1)
	{
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	}
	if (MoveValue.X == -1)
	{
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw - 180, 0.0f));
	}
	if (MoveValue.Y == 1)
	{
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw + 90, 0.0f));
	}
	if (MoveValue.Y == -1)
	{
		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw - 90, 0.0f));
	}

	GSAnim->PlayDodgeMontage();
}

void APlayerCharacter::AttackCheck()
{
	// 충돌된 액터에 관련된 정보를 얻기 위한 구조체
	FHitResult HitResult;
	// 플레이어 자신은 탐색되지 않도록 하기위한 (무시할 액터 목록을 담은)구조체
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
	HitResult,
	GetActorLocation(),
	GetActorLocation() + GetActorForwardVector() * 200.0f,
	FQuat::Identity,
	ECollisionChannel::ECC_GameTraceChannel1,
	FCollisionShape::MakeSphere(50.0f),
	Params);

	// Draw Debug
	#if ENABLE_DRAW_DEBUG

	FVector TraceVector = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVector * 0.5;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
	FColor DrawColor = bResult?FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

	#endif

	if (bResult && HitResult.Actor.IsValid())
	{
		GSLOG(Warning, TEXT("Hit Actor Name : %s"),*HitResult.Actor->GetName());
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), 20.0f, NULL, GetController(), NULL);
	}

}