// Fill out your copyright notice in the Description page of Project Settings.

#include "MDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MDPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "MDProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"



// Sets default values
AMDCharacter::AMDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 1;

	Damage = 10;
	FireRate = 0.5;
	ProjectileDistance = 1000;
	ProjectileSpeed = 450;

	Health = 100;
	MaxHealth = 100;

	LifeRegenAmmount = 1;
	LifeRegenTime = 0.7;

	bCanFire = true;
	bIsDead = false;

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;


	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponSkeletalMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ADD_Parts_HereSocket"));
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called when the game starts or when spawned
void AMDCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController =  Cast<AMDPlayerController>(GetInstigatorController());
	PlayerController->PlayerCharacter = this;
	
}

// Called every frame
void AMDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	Shoot();

}

void AMDCharacter::Move()
{
	const FVector MoveDirection = FVector(PlayerController->MovementForwardValue, PlayerController->MovementRightValue, 0.f).GetClampedToMaxSize(1.0);

	if (MoveDirection.SizeSquared() > 0)
	{
		AddMovementInput(MoveDirection, MovementSpeed);
	}
}

void AMDCharacter::Shoot()
{
	const FVector FireDirection = FVector(PlayerController->ShootingForwardValue, PlayerController->ShootingRightValue, 0.f);
	if (FireDirection.SizeSquared() > 0)
	{
		WeaponSkeletalMesh->SetWorldRotation(FireDirection.Rotation().Add(0.0,0.0,0.0).Quaternion(), true);
	}

	if (!bCanFire)
	{
		return;
	}

	if (PlayerController->bIsConstructing)
	{
		return;
	}

	

	if (FireDirection.SizeSquared() > 0)
	{
		//rotate tower and fire
		bCanFire = false;
		UWorld* World = GetWorld();
		if (World && BulletMesh)
		{
			World->GetTimerManager().SetTimer(ReloadFireTimer, this, &AMDCharacter::ReloadFire, FireRate,false);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = NULL;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform ShootingTransform = this->GetTransform();
			ShootingTransform.SetRotation(FireDirection.Rotation().Quaternion());
			ShootingTransform.SetLocation(WeaponSkeletalMesh->GetBoneLocation(TEXT("Gun_End")));

			AMDProjectile * Projectile = World->SpawnActor<AMDProjectile>(AMDProjectile::StaticClass(), ShootingTransform, SpawnParams);

			Projectile->Setup(ProjectileSpeed,Damage,ProjectileDistance,BulletMesh,this);
			Projectile->Start();

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
		}
	}
}

void AMDCharacter::RegenLife()
{
	ModifyHealth(LifeRegenAmmount);

	if (Health >= MaxHealth)
	{
		//stop the timer for regen
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(HealthRegenTimer);
		}
	}
}

void AMDCharacter::ReloadFire()
{
	bCanFire = true;
}

void AMDCharacter::ReceiveResources(int Ammount)
{
	Resources += Ammount;
}

bool AMDCharacter::CanSpendResources(int Ammount)
{
	return (Resources >= Ammount);
}

void AMDCharacter::SpendResources(int Ammount)
{
	Resources -= Ammount;
}

void AMDCharacter::ModifyHealth(int Ammount)
{
	if (Health + Ammount <= 0)
	{
		Health = 0;
		Die();
	}
	Health = FMath::Clamp(Health + Ammount, (float)0, MaxHealth);

	if (Health < MaxHealth && !HealthRegenTimer.IsValid())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(HealthRegenTimer, this, &AMDCharacter::RegenLife, LifeRegenTime, true);
		}
	}
}

void AMDCharacter::Die()
{
	bIsDead = true;
}

void AMDCharacter::DMTakeDamage(float Damage, AActor* DamageInstigator)
{
	ModifyHealth(-Damage);
}

// Called to bind functionality to input
void AMDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

