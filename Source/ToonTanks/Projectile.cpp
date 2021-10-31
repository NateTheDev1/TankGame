// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));

	TrailParticles->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit
(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	const auto MyOwner = GetOwner();

	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	};

	const auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	const auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage
		(
			OtherActor,
			Damage,
			MyOwnerInstigator,
			this,
			DamageTypeClass
		);

		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation
			(
				this,
				HitParticles,
				GetActorLocation(),
				GetActorRotation()
			);
		}

		if (HitCameraShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShake);
		}
	}

	Destroy();
}
