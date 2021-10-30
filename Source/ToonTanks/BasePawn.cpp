// Nathaniel Richards 2021

#include "BasePawn.h"

#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget) const
{
	const FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	const FRotator LookAtRotation(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	if (ProjectileClass)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>
		(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation()
		);

		Projectile->SetOwner(this);
	}
}
