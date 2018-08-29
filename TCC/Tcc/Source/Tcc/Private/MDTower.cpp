// Fill out your copyright notice in the Description page of Project Settings.

#include "MDTower.h"
#include "MDProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "MDBase.h"



// Sets default values
AMDTower::AMDTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	BaseTowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	//BaseTowerMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("ADD_Parts_HereSocket"));
	BaseTowerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BaseTowerMesh);


	TopTowerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TowerMeshComp"));
	TopTowerMesh->AttachToComponent(BaseTowerMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("ADD_Parts_HereSocket"));
	TopTowerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("ReachRadiusComp"));
	CollisionRadius->SetSphereRadius(MaxRange);

	CollisionRadius->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionRadius->SetCollisionProfileName(FName("OverlapAll"));
	CollisionRadius->bGenerateOverlapEvents = true;

	CollisionRadius->AttachToComponent(BaseTowerMesh,FAttachmentTransformRules::KeepRelativeTransform);

	FScriptDelegate DelegateBeginOverlap;
	DelegateBeginOverlap.BindUFunction(this, "OnOverlapBegin");
	CollisionRadius->OnComponentBeginOverlap.Add(DelegateBeginOverlap);

	FScriptDelegate DelegateEndOverlap;
	DelegateEndOverlap.BindUFunction(this, "OnOverlapEnd");
	CollisionRadius->OnComponentEndOverlap.Add(DelegateEndOverlap);


}

// Called when the game starts or when spawned
void AMDTower::BeginPlay()
{
	Super::BeginPlay();
	bIsConstructing = true;
	
}

// Called every frame
void AMDTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMDTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMDTower::MoveHorizontal(float value)
{
	FVector Location = GetActorLocation();
	Location.X += value * ConstructMoveSpeed;
	SetActorLocation(Location, true);
}

void AMDTower::MoveVertical(float value)
{
	FVector Location = GetActorLocation();
	Location.Y += value * ConstructMoveSpeed;
	SetActorLocation(Location, true);
}

bool AMDTower::IsAbleToConstruct()
{
	return true;
}

void AMDTower::Construct()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult OutHit;
		FVector BeginTrace = GetActorLocation();
		FVector EndTrace = BeginTrace + FVector::UpVector * -1000;

		if (World->LineTraceSingleByChannel(OutHit, BeginTrace, EndTrace, ECollisionChannel::ECC_WorldStatic)) 
		{
			SetActorLocation(OutHit.ImpactPoint);
		}

		BaseTowerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Enemies.Empty();
		CollisionRadius->UpdateOverlaps();
		CollisionRadius->SetSphereRadius(MaxRange);

		TArray<AActor*> OverlappingActors;
		CollisionRadius->GetOverlappingActors(OverlappingActors);

		for (auto& SomeActor : OverlappingActors)
		{
			if (SomeActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()) && SomeActor != this && SomeActor != TowerOwner && SomeActor->GetClass() != this->GetClass())
			{
				if (!Enemies.Contains(SomeActor))
				{
					AMDBase* PossibleBase = Cast<AMDBase>(SomeActor);

					if (IsValid(PossibleBase))
					{
						
					}
					else
					{
						Enemies.Add(SomeActor);
					}
				}
			}

		}
	}
	bIsConstructing = false;
}

void AMDTower::UpdateWeaponry(float NewDamage, float NewRange, float NewFireRate, USkeletalMesh * NewMesh, AActor * NewOwner, float NewAccuracy, float NewAccuracyRadius, float NewMaxHealth, float NewProjectileSpeed, UStaticMesh * NewProjectileMesh)
{
	ProjectileDamage = NewDamage;
	MaxRange = NewRange;
	FireRate = NewFireRate;
	TowerMesh = NewMesh;
	TowerOwner = NewOwner;
	Accuracy = NewAccuracy;
	AccuracyRadius = NewAccuracyRadius;
	MaxHealth = NewMaxHealth;
	ProjectileSpeed = NewProjectileSpeed;
	ProjectileMesh = NewProjectileMesh;
	
	Health = MaxHealth;
	CollisionRadius->SetSphereRadius(MaxRange);
	TopTowerMesh->SetSkeletalMesh(TowerMesh);
}

void AMDTower::Shoot(AActor* Target)
{
	if (bIsAbleToShoot && !bIsConstructing)
	{
		//rotate tower and fire
		bIsAbleToShoot = false;
		UWorld* World = GetWorld();
		if (World && ProjectileMesh)
		{
			World->GetTimerManager().SetTimer(ReloadFireTimer, this, &AMDTower::Reload, FireRate, false);

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
			ProjectileTransform.SetLocation(TopTowerMesh->GetSocketLocation(TEXT("Gun_End")));
			TopTowerMesh->SetWorldRotation(ProjectileTransform.GetRotation());


			AMDProjectile * Projectile = World->SpawnActor<AMDProjectile>(AMDProjectile::StaticClass(),ProjectileTransform, SpawnParams);

			
			Projectile->ShootingActor = this;
			Projectile->Setup(ProjectileSpeed, ProjectileDamage, MaxRange, ProjectileMesh, TowerOwner);
			Projectile->Start();
		}
	}

}

void AMDTower::DMTakeDamage(float Damage, AActor* DamageInstigator)
{
	if (DamageInstigator != TowerOwner)
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

void AMDTower::Die()
{
	Destroy();
}

void AMDTower::Repair()
{
	Health = MaxHealth;
}

void AMDTower::Reload()
{
	bIsAbleToShoot = true;
}

void AMDTower::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()) && OtherActor != this && OtherActor != TowerOwner && OtherActor->GetClass() != this->GetClass()){
		if (!Enemies.Contains(OtherActor))
		{
			AMDBase* PossibleBase = Cast<AMDBase>(OtherActor);

			if (IsValid(PossibleBase))
			{

			}
			else
			{
				Enemies.Add(OtherActor);
			}

		}
	}
}


void AMDTower::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()) && OtherActor != this && OtherActor != TowerOwner && OtherActor->GetClass() != this->GetClass()) {

		if (!Enemies.Contains(OtherActor))
		{
			Enemies.Remove(OtherActor);
		}
	}
}

