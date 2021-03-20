#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(Category = State)
enum class EPlayerState : uint8
{
	Idle,
	Moving,
	Attacking, 
	Parrying,
	Dodge
};

// 마우스 입력 값
UENUM(Category = Attack)
enum class EMouseControll : uint8
{
	None,
	Left,
	Right
};

UCLASS(Blueprintable)
class GREATSWORD_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	// Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class	UCameraComponent* Camera;

	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UPlayerCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class UCapsuleComponent* WeaponCollision;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EPlayerState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EMouseControll CurrentMouseInput;

	// AnimInstance
	UPROPERTY()
	class UPlayer_AnimInstance* PlayerAnim;

	// Weapon
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Attack

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 AttackComboIndex;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 SmashIndex;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FVector MoveValue;

#pragma region Legacy - Draw Debug

	// Draw Debug
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//float AttackRange;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//float AttackRadius;

#pragma endregion

private:

	//Movement
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void Run();
	void Walk();

	//Attack
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupeted);

	void Attack();
	void Smash();
	//void AttackCheck();

	//공격 속성 지정
	void AttackStartComboState();
	void AttackEndComboState();

	//Evade
	void Evade();
	void Parrying();
	void Dodge();

	void SetPlayerRotation();

public : 
	
	int32 GetAttackComboIndex() const {return AttackComboIndex;}
	EPlayerState GetPlayerState() const { return CurrentState; }
	bool CanEvade() const {	return (CurrentState != EPlayerState::Attacking) && (CurrentState != EPlayerState::Parrying) && (CurrentState != EPlayerState::Dodge);}
};
