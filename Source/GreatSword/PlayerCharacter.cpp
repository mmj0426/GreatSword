// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GSAnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(85.0f);
	GetCapsuleComponent()->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));

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
	GetCharacterMovement()->MaxWalkSpeed = 299.0f;

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
	Anim_Player(TEXT("/Game/Blueprints/Animations/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter_C"));
	
	if (Anim_Player.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Player.Class);
	}

	//Weapon

	FName WeaponSocket(TEXT("Weapon_L"));
	if(GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SK_Weapon(TEXT("/Game/GreatSword/GreatSword/Weapon/GreatSword_02.GreatSword_02"));
		
		if (SK_Weapon.Succeeded())
		{
			Weapon->SetStaticMesh(SK_Weapon.Object);
		}
		
		Weapon->SetupAttachment(GetMesh(),WeaponSocket);
	}

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
}

void APlayerCharacter::MoveForward(float NewAxisValue)
{
	if (Controller != nullptr && NewAxisValue != 0.0f)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
	}
}

void APlayerCharacter::MoveRight(float NewAxisValue)
{
	if (Controller != nullptr && NewAxisValue != 0.0f)
	{
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
	GetCharacterMovement()->MaxWalkSpeed = 299.0f;
}

void APlayerCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
}

