// Fill out your copyright notice in the Description page of Project Settings.

#include "MDBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
AMDBase::AMDBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMDBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMDBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMDBase::Die()
{

	UWorld* TheWorld = GetWorld();

	if (TheWorld)
	{
		FString CurrentLevel = TheWorld->GetMapName();

		UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevel));
	}

}

void AMDBase::DMTakeDamage(float Damage, AActor * DamageInstigator)
{
	if (DamageInstigator != BaseOwner)
	{
		if (Health - Damage <= 0)
		{
			Health = 0;
			Die();
		}
		else
		{
			Health -= Damage;
		}
	}
}

