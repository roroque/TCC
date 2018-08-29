// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Damageable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


#include "MDTower.generated.h"

class USphereComponent;
UCLASS()
class TCC_API AMDTower : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMDTower();

	FTimerHandle ReloadFireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAcces = "true"))
	USphereComponent* CollisionRadius;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAcces = "true"))
	UStaticMeshComponent* BaseTowerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAcces = "true"))
	USkeletalMeshComponent* TopTowerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor *> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConstructMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConstructing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAbleToShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TowerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccuracyRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* TowerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveHorizontal(float value);

	void MoveVertical(float value);

	bool IsAbleToConstruct();

	void Construct();

	void UpdateWeaponry(float NewDamage, float NewRange, float NewFireRate, USkeletalMesh* NewMesh, AActor* NewOwner, float NewAccuracy, float NewAccuracyRadius, float NewMaxHealth, float NewProjectileSpeed, UStaticMesh* NewProjectileMesh);

	UFUNCTION(BlueprintCallable)
	void Shoot(AActor* Target);

	virtual void DMTakeDamage(float Damage, AActor* DamageInstigator) override;

	void Die();

	void Repair();

	void Reload();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
