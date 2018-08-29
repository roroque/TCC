// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MDProjectile.generated.h"



class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class TCC_API AMDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMDProjectile();

	UPROPERTY()
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	UBoxComponent* CollisionComp;


	UPROPERTY()
	float ProjectileSpeed;

	UPROPERTY()
	float ProjectileDamage;

	UPROPERTY()
	float ProjectileDistance;

	UPROPERTY()
	AActor *ShootingOwner;

	UPROPERTY()
	AActor *ShootingActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Setup(float Speed, float Damage, float MaxDistance, UStaticMesh* Mesh, AActor* MyOwner);

	UFUNCTION()
	void Start();
	
};
