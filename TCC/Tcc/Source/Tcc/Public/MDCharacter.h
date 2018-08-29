// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Damageable.h"

#include "MDCharacter.generated.h"


class AMDPlayerController;

UCLASS()
class TCC_API AMDCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()




public:
	// Sets default values for this character's properties
	AMDCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AMDPlayerController * PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* BulletMesh;

	//meshes for weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeRegenTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeRegenAmmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resources;

	bool bCanFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead;

	//timers
	FTimerHandle HealthRegenTimer;
	FTimerHandle ReloadFireTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//event for controlling movement
	void Move();

	//event for controlling shooting
	void Shoot();

	void RegenLife();

	void ReloadFire();

	void ReceiveResources(int Ammount);

	bool CanSpendResources(int Ammount);

	void SpendResources(int Ammount);

	void ModifyHealth(int Ammount);

	void Die();

	//Take damage function
	virtual void DMTakeDamage(float Damage, AActor* DamageInstigator) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	
	
};
