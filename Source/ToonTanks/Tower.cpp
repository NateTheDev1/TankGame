// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Tower.h"


void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer
	(
		FireRateTimerHandle,
		this,
		&ATower::CheckFireCondition,
		FireRate,
		true
	);
}

void ATower::CheckFireCondition()
{
	if (Tank)
	{
		const bool InRange = CheckIfInRange();

		if (InRange)
		{
			Fire();
		}
	}
}

bool ATower::CheckIfInRange() const
{
	const float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

	return Distance <= FireRange;
}

void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Tank)
	{
		const bool InRange = CheckIfInRange();

		if (InRange)
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}
