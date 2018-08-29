// Fill out your copyright notice in the Description page of Project Settings.

#include "MDProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Damageable.h"
#include "MDTower.h"


// Sets default values
AMDProjectile::AMDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->InitBoxExtent(FVector(10.0f, 10.0f, 10.0f));
	//CollisionComp->SetHiddenInGame(false); //use this for debug
	SetRootComponent(CollisionComp);
	//CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionProfileName(FName("OverlapAll"));
	CollisionComp->bGenerateOverlapEvents = false;

	FScriptDelegate DelegateBeginOverlap;
	DelegateBeginOverlap.BindUFunction(this, "OnOverlapBegin");
	CollisionComp->OnComponentBeginOverlap.Add(DelegateBeginOverlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachToComponent(CollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

// Called when the game starts or when spawned
void AMDProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMDProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMDProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->GetClass() != UStaticMeshComponent::StaticClass() && OtherComp->GetClass() != USkeletalMeshComponent::StaticClass() && OtherComp->GetClass() != UCapsuleComponent::StaticClass())
	{
		return;
	}

	AMDTower *PossibleTower = Cast<AMDTower>(OtherActor);
	if (PossibleTower && (PossibleTower->TowerOwner == ShootingOwner || PossibleTower->TowerOwner == ShootingActor) )
	{
		return;
	}

	if (ShootingActor && OtherActor->GetClass() == ShootingActor->GetClass())
	{
		return;
	}


	if (ShootingOwner && OtherActor != this && OtherActor != ShootingOwner && OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass())) {
		if (ShootingActor && ShootingActor == OtherActor)
		{
			return;
		}
		TScriptInterface<IDamageable> DamageableActor;
		DamageableActor.SetInterface(Cast<IDamageable>(OtherActor));
		DamageableActor.SetObject(OtherActor);
		//make this receive type of damage
		DamageableActor->DMTakeDamage(ProjectileDamage,ShootingOwner);

		Destroy();
	}
	if (ShootingOwner && OtherActor != this && OtherActor != ShootingOwner)
	{
		Destroy();
	}
}

void AMDProjectile::Setup(float Speed, float Damage, float MaxDistance, UStaticMesh* Mesh, AActor* MyOwner)
{
	ShootingOwner = MyOwner;
	ProjectileDamage = Damage;
	ProjectileSpeed = Speed;
	ProjectileDistance = MaxDistance;

	if (Mesh)
	{
		StaticMeshComp->SetStaticMesh(Mesh);
		StaticMeshComp->AttachToComponent(CollisionComp, FAttachmentTransformRules::SnapToTargetIncludingScale);

		CollisionComp->SetBoxExtent(Mesh->ExtendedBounds.BoxExtent);
	}
}

void AMDProjectile::Start()
{
	TArray<AActor*> OverlappingActors;
	CollisionComp->GetOverlappingActors(OverlappingActors);

	for (auto& SomeActor : OverlappingActors)
	{
		if (ShootingOwner && SomeActor != this && SomeActor != ShootingOwner && SomeActor->GetClass()->ImplementsInterface(UDamageable::StaticClass())) {

			TScriptInterface<IDamageable> DamageableActor;
			DamageableActor.SetInterface(Cast<IDamageable>(SomeActor));
			DamageableActor.SetObject(SomeActor);
			//make this receive type of damage
			DamageableActor->DMTakeDamage(ProjectileDamage,ShootingOwner);
			break;
		}
	}

	CollisionComp->bGenerateOverlapEvents = true;
	CollisionComp->UpdateOverlaps();

	ProjectileMovement->SetVelocityInLocalSpace(FVector(ProjectileSpeed, 0, 0));


}

