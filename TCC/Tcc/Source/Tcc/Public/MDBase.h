// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "MDBase.generated.h"

UCLASS()
class TCC_API AMDBase : public AActor,public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMDBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* BaseOwner;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();

	//Take damage function
	virtual void DMTakeDamage(float Damage, AActor* DamageInstigator) override;
	
	
};
