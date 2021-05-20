#pragma once

#include "GreatSword.h"
#include "GameFramework/Character.h"

#include "Boss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1 = 0 UMETA(DisplayName = "Phase1"),
	Phase2 UMETA(DisplayName = "Phase2"),
	Phase3 UMETA(DisplayName = "Phase3")
};

UCLASS(Blueprintable)
class GREATSWORD_API ABoss : public ACharacter
{
	GENERATED_BODY()

public: 
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents()override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void BossAttack();
	void DecideAttackType();

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupeted);

	FOnAttackEndDelegate OnAttackEnd;

public: 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	class UBossStatComponent* BossStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	bool IsAlive;


	//UPROPERTY(EditDefaultsOnly, Category = Stat)
	//	float MaxHP;

	//UPROPERTY(VisibleAnywhere, Category = Stat)
	//	float CurrentHP;

	EBossPhase GetCurrentPhase() { return CurrentPhase; }

	void SetCurrentPhase(EBossPhase NewPhase) { CurrentPhase = NewPhase; }

	//float GetMaxHP() { return MaxHP; }
	//float GetCurrentHP() { return CurrentHP; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	bool CanDash;


private : 

	// AnimInstance
	UPROPERTY()
	class UBoss_AnimInstance* BossAnim;

	UPROPERTY()
	FString CurrentAttackType;

	UPROPERTY(EditAnywhere, Category = Phase)
	EBossPhase CurrentPhase;

	int32 judgeAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float HitDamage;

	UPROPERTY()
	bool IsPhase2FirstEntry;

	UPROPERTY()
	bool IsPhase3FirstEntry;


};
