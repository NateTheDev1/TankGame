// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"
#include "ToonTanksGameMode.h"

#define OUT

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;

		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject
		(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);

		GetWorldTimerManager().SetTimer
		(
			PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}
}

int32 AToonTanksGameMode::GetTargetTowerCount() const
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), OUT Towers);

	return Towers.Num();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();

		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}

		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		--TargetTowers;

		if(TargetTowers == 0)
		{
			GameOver(true);
		}
	}
}
