// Fill out your copyright notice in the Description page of Project Settings.

#include "MDEnemy.h"
#include "MDProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/CollisionProfile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"


// Sets default values
AMDEnemy::AMDEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*
	BodyCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	BodyCapsule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BodyCapsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	BodyCapsule->SetSimulatePhysics(true);
	BodyCapsule->SetEnableGravity(true);
	BodyCapsule->SetConstraintMode(EDOFMode::XYPlane);
	BodyCapsule->SetLockedAxis(EDOFMode::XYPlane);
	BodyCapsule->GetBodyInstance()->bLockRotation = true;


	//this creates the mesh
	BodySkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMeshComp"));
	BodySkeletalMesh->AttachToComponent(BodyCapsule, FAttachmentTransformRules::KeepRelativeTransform);
	BodySkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	*/

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponSkeletalMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,TEXT("ADD_Parts_HereSocket"));
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AMDEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMDEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMDEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMDEnemy::DMTakeDamage(float Damage, AActor *DamageInstigator)
{
	if (Health - Damage <= 0)
	{
		Health = 0;
		Die();
	}
	else {
		Health -= Damage;
	}
}

void AMDEnemy::Die()
{
	bIsDead = true;
	GetCapsuleComponent()->bGenerateOverlapEvents = false;
	AAIController* ThisController = Cast<AAIController>(GetController());
	ThisController->BrainComponent->StopLogic("Dead");
	

	//BodyCapsule->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

}

void AMDEnemy::Reload()
{

	bIsAbleToShoot = true;

}

void AMDEnemy::Shoot(AActor* Target)
{

	if (bIsAbleToShoot)
	{

		bIsAbleToShoot = false;
		UWorld* World = GetWorld();
		if (World && ProjectileMesh)
		{
			World->GetTimerManager().SetTimer(ReloadFireTimer, this, &AMDEnemy::Reload, ReloadSpeed, false);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = NULL;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform ProjectileTransform = this->GetTransform();

			FVector ShootingVectortarget = Target->GetActorLocation();
			ShootingVectortarget.Z = 0;

			FVector ShootingVectorMine = this->GetActorLocation();
			ShootingVectorMine.Z = 0;

			FVector ShootingVector = ShootingVectortarget - ShootingVectorMine;

			ProjectileTransform.SetRotation(ShootingVector.Rotation().Quaternion());
			ProjectileTransform.SetLocation(WeaponSkeletalMesh->GetSocketLocation(TEXT("Gun_End")));
			WeaponSkeletalMesh->SetWorldRotation(ProjectileTransform.GetRotation());



			AMDProjectile * Projectile = World->SpawnActor<AMDProjectile>(AMDProjectile::StaticClass(), ProjectileTransform, SpawnParams);

			Projectile->Setup(ProjectileSpeed, ProjectileDamage,MaxReach, ProjectileMesh,this);
			Projectile->Start();
		}

	}
}

