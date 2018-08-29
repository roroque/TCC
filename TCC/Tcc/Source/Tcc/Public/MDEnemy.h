// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Damageable.h"
#include "MDEnemy.generated.h"

UCLASS()
class TCC_API AMDEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMDEnemy();

	FTimerHandle ReloadFireTimer;


	//meshes for capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* BodyCapsule;

	//meshes for body
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BodySkeletalMesh;

	//meshes for weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAbleToShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxReach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Precision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrecisionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ProjectileMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	

	// Herdado por meio de IDamageable
	virtual void DMTakeDamage(float Damage, AActor *DamageInstigator);

	UFUNCTION(BlueprintCallable)
	void Die();

	void Reload();

	UFUNCTION(BlueprintCallable)
	void Shoot(AActor* Target);

};
