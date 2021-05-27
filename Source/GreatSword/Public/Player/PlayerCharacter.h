#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(Category = Attack)
enum class EReadyToAttack
{
	None,
	Attack,
	Smash
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
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* Camera;

	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	class UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UPlayerCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class UCapsuleComponent* WeaponCollision;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	EReadyToAttack CurrentMouseInput;

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

	// Recovery
	bool bCanHP_Recovery;
	bool bCanStamina_Recovery;



#pragma region Legacy - Draw Debug

	// Draw Debug
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//float AttackRange;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//float AttackRadius;

#pragma endregion

public:

	FTimerHandle HP_RecoveryCheckHandle;

	FTimerHandle Stamina_RecoveryCheckHandle;

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

	void UseStamina(bool isAttack);

	// Recovery
	bool GetCanHP_Recovery() {return bCanHP_Recovery;}
	bool GetCanStamina_Recovery() { return bCanStamina_Recovery; }

public:
	
	int32 GetAttackComboIndex() const {return AttackComboIndex;}

	bool CanEvade() const;
	bool CanMove() const;
	bool CanAttack() const;


};
