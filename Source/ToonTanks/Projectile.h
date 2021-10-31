// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void OnHit
	(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> HitCameraShake;
};
