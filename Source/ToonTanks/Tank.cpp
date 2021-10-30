// Nathaniel Richards 2021


#include "Tank.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

#define OUT

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10000, FColor::Yellow, TEXT("Press E To Boost"));

	AudioComp = FindComponentByClass<UAudioComponent>();

	if (!AudioComp)
	{
		UE_LOG(LogTemp, Error, TEXT("No audio component!"));
	}

	PlayerControllerRef = Cast<APlayerController>(GetController());

	if (!PlayerControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("No Player controller"));
	}
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &ATank::Boost);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value)
{
	FVector DeltaLocation(0.f);
	DeltaLocation.X = Value * GetWorld()->GetDeltaSeconds() * Speed;
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * TurnRate * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ValidateBoostAbility();

	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, true, OUT HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();


	SetActorHiddenInGame(true);

	SetActorTickEnabled(false);
}


void ATank::Boost()
{
	if (!Boosting)
	{
		if (AudioComp)
		{
			AudioComp->Play();
		}

		GEngine->ClearOnScreenDebugMessages();
		Boosting = true;
		TimeSinceLastBoost = GetWorld()->GetTimeSeconds();
		Speed += 400.f;
	}
}

void ATank::ValidateBoostAbility()
{
	if (Boosting)
	{
		if (GEngine)
		{
			GEngine->ClearOnScreenDebugMessages();
			const int32 SecondsLeft = static_cast<int32>(TimeSinceLastBoost + BoostLength - GetWorld()->
				GetTimeSeconds()) + 1;
			const FString BoostMessage = FString::Printf(
				TEXT("%i seconds left boosting"), SecondsLeft);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, BoostMessage);

			if (TimeSinceLastBoost + BoostLength <= GetWorld()->GetTimeSeconds())
			{
				GEngine->ClearOnScreenDebugMessages();
				Boosting = false;
				Speed -= 400.f;
				GEngine->AddOnScreenDebugMessage(-1, 10000, FColor::Yellow, TEXT("Press E To Boost"));
			}
		}
	}
}
