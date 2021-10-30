// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"


void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
}


void ATower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Tank)
	{
		const float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		if (Distance <= FireRange)
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
}
