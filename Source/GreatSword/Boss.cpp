// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(140.0f);
	GetCapsuleComponent()->SetCapsuleRadius(60.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Boss");

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-130.0f),FRotator(0.0f,-90.0f,0.0f));

	MaxHP = 100.0f;

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

	CurrentHP -= Damage;

	GSLOG(Warning, TEXT("Actor : %s, HP : %f"), *GetName(), CurrentHP);

	if (CurrentHP <= 0)
	{
		//TODO : Boss Die
		GSLOG(Warning, TEXT("Boss Die"));

		return Damage;
	}

	return Damage;
}
